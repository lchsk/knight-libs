#pragma once

#include <memory>
#include <queue>
#include <unordered_map>

#include <SFML/Graphics.hpp>

namespace K {
class Loader {
  public:
    void load_textures();
    Loader *add_texture(const std::string &, const std::string &);
    const sf::Texture &get_texture(const std::string &filename) const;

  private:
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
    std::queue<std::pair<std::string, std::string>> textures_queue;
};
}
