#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "../src/loader.hpp"

namespace K {
struct Game {
    Game();

    void set_main_view(sf::View &&);
    void set_minimap_view(sf::View &&);
    void set_gui_view(sf::View &&);

    sf::View *get_minimap_view() const;
    sf::View *get_main_view() const;
    sf::View *get_gui_view() const;

    sf::RenderWindow& get_window();
    K::Loader& get_loader();

    std::unique_ptr<sf::RenderWindow> window;
    std::unique_ptr<K::Loader> loader;


    std::unique_ptr<sf::View> minimap_view;
    std::unique_ptr<sf::View> main_view;
    std::unique_ptr<sf::View> gui_view;
};
}
