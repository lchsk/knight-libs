#include "loader.hpp"

namespace K {
Loader *Loader::add_texture(const std::string &identifier,
                            const std::string &path) {
    textures_queue.push(std::make_pair(identifier, path));

    return this;
}

const sf::Texture &Loader::get_texture(const std::string &filename) const {
    return *textures.at(filename);
}

void Loader::load_textures() {
    while (!textures_queue.empty()) {
        const auto pair = textures_queue.front();
        textures_queue.pop();

        const auto identifier = pair.first;
        const auto path = pair.second;

        textures[identifier] = std::make_unique<sf::Texture>();

        if (!textures[identifier]->loadFromFile(path)) {
            throw std::runtime_error("Unable to load " + path);
        }
    }
}

void Loader::load_music() {
    while (!music_queue.empty()) {
        const auto pair = music_queue.front();
        music_queue.pop();

        const auto identifier = pair.first;
        const auto path = pair.second;

        music[identifier] = std::make_unique<sf::Music>();

        if (!music[identifier]->openFromFile(path)) {
            throw std::runtime_error("Unable to load " + path);
        }
    }
}

Loader *Loader::add_music(const std::string &identifier,
                          const std::string &path) {
    music_queue.push(std::make_pair(identifier, path));
}
sf::Music *Loader::get_music(const std::string &filename) const {
    return music.at(filename).get();
}
}
