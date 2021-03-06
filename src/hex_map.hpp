///////////////////////////////////////////////////////////////////////////////
//                                 Hex Map                                   //
///////////////////////////////////////////////////////////////////////////////

// https://github.com/lchsk/knight-libs
// v0.2.0

#pragma once

#include <limits>
#include <sstream>
#include <unordered_map>
#include <vector>

#include <SFML/Graphics.hpp>

#include "../src/simple_ecs.hpp"
#include "animation.hpp"
#include "hex.hpp"
#include "loader.hpp"
#include <iostream>

#define HEX_MAP_LIMIT_CALC(min, max, size, speed, extra)                       \
    (max - size - min - speed + extra)

namespace K {
class HexCamera {
  public:
    HexCamera(K::Point origin, K::Point size, K::Point hex_size, sf::View *view)
        : origin(origin), size(size), view(view), hex_size(hex_size) {}

    void update(const sf::Vector2i &pos) {
        double dx = 0, dy = 0;

        if (pos.x < (size.x * window_pct) && pos.x > 0 &&
            origin.x >= pos_min.x) {
            // Move left
            dx = -speed;
        }

        if (pos.y < (size.y * window_pct) && pos.y > 0 &&
            origin.y > pos_min.y) {
            // Move up
            dy = -speed;
        }

        const auto limit_x = HEX_MAP_LIMIT_CALC(pos_min.x, pos_max.x, size.x,
                                                speed, hex_size.x / 2);
        const auto limit_y =
            HEX_MAP_LIMIT_CALC(pos_min.y, pos_max.y, size.y, speed, hex_size.y);

        if (pos.x > (size.x * (1 - window_pct)) && pos.x < size.x &&
            origin.x < limit_x) {
            // Move right
            dx = speed;
        }

        if (pos.y > (size.y * (1 - window_pct)) && pos.y < size.y &&
            origin.y < limit_y) {
            // Move down
            dy = speed;
        }

        if (NON_ZERO_DOUBLE(dx) or NON_ZERO_DOUBLE(dy)) {
            origin.x += dx;
            origin.y += dy;
            view->move(dx, dy);
        }
    }

    const sf::View &get_view() const { return *view; }

    const K::Point &get_pos_min() const { return pos_min; }

    const K::Point &get_pos_max() const { return pos_max; }

    void set_speed(double speed_) { speed = speed_; }

    void set_window_pct(double window_pct_) { window_pct = window_pct_; }

    void find_limits(
        const K::Layout &layout,
        const std::unordered_map<K::Hex, std::vector<std::string>> &hexes) {
        pos_max.x = pos_max.y = std::numeric_limits<double>::min();
        pos_min.x = pos_min.y = std::numeric_limits<double>::max();

        for (const auto &hex : hexes) {
            auto &hex_coords = hex.first;

            const auto p = hex_coords.to_point(layout);

            if (p.x < pos_min.x)
                pos_min.x = p.x;
            if (p.y < pos_min.y)
                pos_min.y = p.y;
            if (p.x > pos_max.x)
                pos_max.x = p.x;
            if (p.y > pos_max.y)
                pos_max.y = p.y;
        }
    }

  private:
    K::Point pos_min, pos_max;
    sf::View *view;
    K::Point origin;
    K::Point size;
    const K::Point hex_size;
    double speed = 3;
    double window_pct = 0.15;
};

template <typename Node> class HexMap {
  public:
    HexMap<Node>(const K::Layout &layout, const K::HexCamera &camera,
                 const K::Point hex_size, sf::View *minimap)
        : layout(layout), camera(camera), hex_size(hex_size), minimap(minimap) {
    };

    K::Point get_hex_middle(const K::Hex &hex) const {
        auto p = hex.to_point(layout);

        p.x += hex_size.x / 2;
        p.y += hex_size.y / 2;

        return p;
    }

    void set_view(sf::RenderWindow &window) {
        window.setView(camera.get_view());
    }

#ifdef KNIGHT_DEBUG
    void set_font(const sf::Font &font_) { font = font_; }
#endif

    void update(const sf::Time &delta, const sf::Vector2i &pos) {
        camera.update(pos);

        const auto view = ecs::VectorView::create<Node>(system);

        for (ecs::Entity *e : view.entities()) {
            auto &component = e->get<Node>();

            for (auto &anim : component->animations) {
                anim.update(delta);
            }
        }
    }

    void draw_tiles(sf::RenderWindow &window) {
        const auto view = ecs::VectorView::create<Node>(system);

        for (ecs::Entity *e : view.entities()) {
            auto &component = e->get<Node>();

            for (auto &anim : component->animations) {
                anim.render(window);
            }
        }
    }

    void render(sf::RenderWindow &window) {
        draw_tiles(window);

#ifdef KNIGHT_DEBUG
        for (const auto &t : hex_positions) {
            window.draw(t);
        }
#endif
    }

#ifdef KNIGHT_DEBUG
    void add_debug_text(const K::Hex &hex, const K::Point &point) {
        sf::Text text;

        text.setFont(font);

        std::stringstream str;

        str << static_cast<int>(hex.q) << "\t" << static_cast<int>(hex.r)
            << "\t" << static_cast<int>(hex.s);

        text.setCharacterSize(12);
        text.setString(str.str());

        text.setPosition(point.x + hex_size.x / 2 + layout.origin.x / 2 -
                             hex_size.x / 4,
                         point.y + hex_size.y / 2);

        hex_positions.push_back(text);
    }
#endif

    void add_object(const ecs::Entity &entity) { entities.push_back(entity); }

    void set_hexes(
        const K::Loader &loader,
        const std::unordered_map<K::Hex, std::vector<std::string>> &hexes_) {
        hexes = hexes_;

        camera.find_limits(layout, hexes);

        ecs::System new_system;
        system = new_system;
        entities.clear();

        for (const auto &hex_data : hexes) {
            const auto &hex = hex_data.first;
            const auto &identifiers = hex_data.second;

            auto entity = system.create();

            const auto point = hex.to_point(layout);

#ifdef KNIGHT_DEBUG
            add_debug_text(hex, point);
#endif

            std::vector<K::Animation> animations;

            for (const auto &identifier : identifiers) {
                K::Animation anim(loader, std::vector<std::string>{identifier});
                anim.set_position(point.x, point.y);

                animations.push_back(anim);
            }

            system.add<Node>(entity, animations);
        }
    }

    const K::Layout &get_layout() const { return layout; }

    const K::Point &get_hex_size() const { return hex_size; }

  private:
    K::Layout layout;
    std::unordered_map<K::Hex, std::vector<std::string>> hexes;
    K::Point hex_size;
    ecs::System system;
    std::vector<ecs::Entity> entities;
    K::HexCamera camera;

    sf::View *minimap;

#ifdef KNIGHT_DEBUG
    sf::Font font;
    std::vector<sf::Text> hex_positions;
#endif
};
}
