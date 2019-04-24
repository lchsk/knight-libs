///////////////////////////////////////////////////////////////////////////////
//                                 Animation                                 //
///////////////////////////////////////////////////////////////////////////////

// https://github.com/lchsk/knight-libs
// v0.2.0

#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "loader.hpp"

// TODO: Move Point etc outside of Hex
#include "hex.hpp"

namespace K {
class Animation {
  public:
    enum class Status {
        Playing,
        Stopped,
        Paused,
    };

    Animation();
    Animation(const std::vector<const sf::Texture *> &);
    Animation(const K::Loader &, const std::vector<std::string> &);
    Animation(const K::Loader &, const std::vector<std::string> &,
              const sf::IntRect &);

    void render(sf::RenderWindow &window);
    void update(const sf::Time &delta);

    K::Animation::Status get_status() const;
    bool is_visible() const;
    void set_visible(bool);

    const K::Point get_sprite_size() const;
    const sf::FloatRect get_sprite_bounds() const;

    bool is_looping() const;
    void set_loop(bool);

    void set_speed(double);
    void set_position(double, double);
    void move(double, double);
    void set_scale(double, double);
    void set_origin(double, double);

    const sf::Vector2f &get_position() const;
    const sf::Vector2f &get_scale() const;

    void play();
    void pause();
    void stop();

    const std::size_t frames_count() const;

  private:
    void init();

    std::vector<sf::Sprite> frames;
    int current_frame;
    sf::Time since_update;
    double speed;
    sf::Vector2f position;
    sf::Vector2f scale;
    sf::Vector2f origin;
    Status status;
    bool visible;
    bool loop;
};
}
