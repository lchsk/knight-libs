#pragma once

#include <memory>
#include <queue>
#include <unordered_map>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

namespace K {
class Loader {
  public:
    void load_textures();
    Loader *add_texture(const std::string &, const std::string &);
    const sf::Texture &get_texture(const std::string &filename) const;

    void load_fonts();
    Loader *add_font(const std::string &, const std::string &);
    const sf::Font &get_font(const std::string &filename) const;

    void load_music();
    Loader *add_music(const std::string &, const std::string &);
    sf::Music *get_music(const std::string &filename) const;

  private:
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
    std::queue<std::pair<std::string, std::string>> textures_queue;

    std::unordered_map<std::string, std::unique_ptr<sf::Font>> fonts;
    std::queue<std::pair<std::string, std::string>> fonts_queue;

    std::unordered_map<std::string, std::unique_ptr<sf::Music>> music;
    std::queue<std::pair<std::string, std::string>> music_queue;
};
}
