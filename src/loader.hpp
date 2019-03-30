#pragma once

#include <memory>
#include <queue>
#include <unordered_map>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

namespace K {
class Loader {
  public:
    void load();

    void load_textures();
    Loader *add_texture(const std::string &, const std::string &);
    const sf::Texture *get_texture(const std::string &filename) const;

    void load_fonts();
    Loader *add_font(const std::string &, const std::string &);
    const sf::Font &get_font(const std::string &filename) const;

    void load_music();
    Loader *add_music(const std::string &, const std::string &);
    sf::Music *get_music(const std::string &filename) const;

    void load_sound_buffers();
    Loader *add_sound_buffer(const std::string &, const std::string &);
    const sf::SoundBuffer &get_sound_buffer(const std::string &filename) const;
    sf::Sound *get_sound(const std::string &filename) const;
    void insert_sound(const std::string &, const std::string &);

  private:
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
    std::queue<std::pair<std::string, std::string>> textures_queue;

    std::unordered_map<std::string, std::unique_ptr<sf::Font>> fonts;
    std::queue<std::pair<std::string, std::string>> fonts_queue;

    std::unordered_map<std::string, std::unique_ptr<sf::Music>> music;
    std::queue<std::pair<std::string, std::string>> music_queue;

    std::unordered_map<std::string, std::unique_ptr<sf::SoundBuffer>>
        sound_buffers;
    std::queue<std::pair<std::string, std::string>> sound_buffers_queue;

    std::unordered_map<std::string, std::unique_ptr<sf::Sound>> sounds;
};
}
