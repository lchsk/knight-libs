#pragma once

#include <unordered_map>
#include <vector>

#include <SFML/Graphics.hpp>

#include "../src/simple_ecs.hpp"
#include "animation.hpp"
#include "hex.hpp"
#include "loader.hpp"

namespace K {
template <typename Node> class HexMap {
  public:
    HexMap<Node>(const K::Layout &layout) : layout(layout){};

    void update(const sf::Time &delta) {
        const auto view = ecs::VectorView::create<Node>(system);

        for (ecs::Entity *e : view.entities()) {
            auto &component = e->get<Node>();

            for (auto &anim : component->animations) {
                anim.update(delta);
            }
        }
    }

    void render(sf::RenderWindow &window) {
        const auto view = ecs::VectorView::create<Node>(system);

        for (ecs::Entity *e : view.entities()) {
            auto &component = e->get<Node>();

            for (auto &anim : component->animations) {
                anim.render(window);
            }
        }
    }

    void set_hexes(
        const K::Loader &loader,
        const std::unordered_map<K::Hex, std::vector<std::string>> &hexes_) {
        hexes = hexes_;

        ecs::System new_system;
        system = new_system;
        entities.clear();

        for (const auto &hex_data : hexes) {
            const auto &hex = hex_data.first;
            const auto &identifiers = hex_data.second;

            auto entity = system.create();

            const auto point = hex.to_point(layout);

            std::vector<K::Animation> animations;

            for (const auto &identifier : identifiers) {
                K::Animation anim(loader, std::vector<std::string>{identifier});
                anim.set_position(point.x, point.y);

                animations.push_back(anim);
            }

            system.add<Node>(entity, animations);
        }
    }

  private:
    K::Layout layout;
    std::unordered_map<K::Hex, std::vector<std::string>> hexes;
    ecs::System system;
    std::vector<ecs::Entity> entities;
};
}
