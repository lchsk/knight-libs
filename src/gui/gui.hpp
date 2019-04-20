#pragma once

#include <unordered_map>
#include <vector>

#include <SFML/Graphics.hpp>

#include "../../src/animation.hpp"
#include "../../src/simple_ecs.hpp"

#if SFML_VERSION_MINOR >= 4
#define SFML_SET_COLOR setFillColor
#else
#define SFML_SET_COLOR setColor
#endif

namespace K {
struct Widget {
    virtual void update(const sf::Time &delta, const sf::Vector2f &mouse_pos){};
    virtual void render(sf::RenderWindow &window){};
};
struct Button : public Widget {
    std::unordered_map<std::string, K::Animation> animations;
    std::string active_animation;

    bool mouse_pressed = false;
    bool mouse_hover = false;
    bool mouse_released = false;
    sf::Text text;

    // Styles
    int default_size = 14;
    int pressed_size = 14;
    int hover_size = 14;

    sf::Color default_color = sf::Color::White;
    sf::Color pressed_color = sf::Color::White;
    sf::Color hover_color = sf::Color::White;

    void set_position(const K::Point &p) {
        for (auto &anim_item : animations) {
            anim_item.second.set_position(p.x, p.y);
        }

        update_text_position();
    }

    void update(const sf::Time &delta, const sf::Vector2f &mouse_pos,
                sf::Event &event) {
        animations[active_animation].update(delta);

        if (mouse_pressed and animations.find("pressed") != animations.end()) {
            active_animation = "pressed";
            text.setCharacterSize(pressed_size);
            text.SFML_SET_COLOR(pressed_color);
        } else if (mouse_hover and
                   animations.find("hover") != animations.end()) {
            active_animation = "hover";
            text.setCharacterSize(hover_size);
            text.SFML_SET_COLOR(hover_color);
        } else {
            active_animation = "default";
            text.setCharacterSize(default_size);
            text.SFML_SET_COLOR(default_color);
        }
    }

    void render(sf::RenderWindow &window) {
        animations[active_animation].render(window);
        window.draw(text);
    }

    void check_mouse_released(const sf::Vector2f &pos, sf::Event &event) {
        if (mouse_pressed_for_release &&
            !sf::Mouse::isButtonPressed(sf::Mouse::Left) && check_bounds(pos)) {
            mouse_released = true;
            mouse_pressed_for_release = false;
        }
    }
    void check_mouse_pressed(const sf::Vector2f &pos, sf::Event &event) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && check_bounds(pos)) {
            mouse_pressed = true;
            mouse_pressed_for_release = true;
        } else {
            mouse_pressed = false;
        }
    }

    void check_mouse_hover(const sf::Vector2f &pos, sf::Event &event) {
        mouse_hover = check_bounds(pos);
    }

    void set_text(const std::string &str) {
        text.setString(str);
        update_text_position();
    }

  private:
    void update_text_position() {
        const auto &bounds = animations[active_animation].get_sprite_bounds();
        const auto &pos = animations[active_animation].get_position();
        const auto &bounds_text = text.getGlobalBounds();

        text.setPosition(pos.x + bounds.width / 2 - bounds_text.width / 2,
                         pos.y + bounds.height / 2 - bounds_text.height / 2);
    }
    bool check_bounds(const sf::Vector2f &pos) {
        const auto &bounds = animations[active_animation].get_sprite_bounds();

        return (pos.x >= bounds.left and pos.x <= bounds.left + bounds.width and
                pos.y >= bounds.top and pos.y <= bounds.top + bounds.height);
    }

    bool mouse_pressed_for_release = false;
};

struct Grid {
    K::Point window_size;
    K::Point grid_size;

    void init_grid(const K::Point &ws, const K::Point &gs) {
        window_size = ws;
        grid_size = gs;

        grid_unit.x = window_size.x / grid_size.x;
        grid_unit.y = window_size.y / grid_size.y;
    }

    K::Point get_position(const K::Point &grid_pos) {
        if (grid_pos.x < 0 or grid_pos.y < 0 or grid_pos.x >= grid_size.x or
            grid_pos.y >= grid_size.y) {
            throw std::runtime_error("grid position is not valid");
        }

        K::Point pos;
        pos.x = grid_unit.x * grid_pos.x;
        pos.y = grid_unit.y * grid_pos.y;

        return pos;
    }

  private:
    K::Point grid_unit;
};

class Gui {
  public:
    void update(const sf::Time &delta, const sf::Vector2f &mouse_pos,
                sf::Event &event) {
        const auto view = ecs::VectorView::create<Button>(system);

        for (ecs::Entity *e : view.entities()) {
            auto &widget = e->get<Button>();

            widget->mouse_released = false;
            widget->check_mouse_pressed(mouse_pos, event);
            widget->check_mouse_hover(mouse_pos, event);
            widget->check_mouse_released(mouse_pos, event);

            widget->update(delta, mouse_pos, event);
        }
    }

    void render(sf::RenderWindow &window) {
        const auto view = ecs::VectorView::create<Button>(system);

        for (ecs::Entity *e : view.entities()) {
            auto &widget = e->get<Button>();
            widget->render(window);
        }
    }

    std::vector<ecs::Entity *> widgets;
    ecs::System system;
    K::Grid grid;

  private:
};
}
