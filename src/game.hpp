#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

namespace K {
class Game {
  public:
    Game();

    void set_main_view(sf::View &&);
    void set_minimap_view(sf::View &&);

    sf::View *get_minimap_view() const;
    sf::View *get_main_view() const;

  private:
    std::unique_ptr<sf::View> minimap_view;
    std::unique_ptr<sf::View> main_view;
};
}
