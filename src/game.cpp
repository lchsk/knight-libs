#include "game.hpp"

namespace K {
Game::Game() {}

void Game::set_main_view(sf::View &&view) {
    main_view = std::make_unique<sf::View>(view);
}

void Game::set_minimap_view(sf::View &&view) {
    minimap_view = std::make_unique<sf::View>(view);
}

sf::View *Game::get_minimap_view() const { return minimap_view.get(); }

sf::View *Game::get_main_view() const { return main_view.get(); }
}
