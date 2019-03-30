#include "animation.hpp"

namespace K {
Animation::Animation(const std::vector<const sf::Texture *> &textures) {
    for (const auto texture : textures) {
        frames.push_back(sf::Sprite(*texture));
    }

    init();
}

Animation::Animation(const K::Loader &loader,
                     const std::vector<std::string> &identifiers) {
    for (const auto &identifier : identifiers) {
        frames.push_back(sf::Sprite(*loader.get_texture(identifier)));
    }

    init();
}

K::Animation::Status Animation::get_status() const { return status; }

bool Animation::is_visible() const { return visible; }

void Animation::set_visible(bool v) { visible = v; }

bool Animation::is_looping() const { return loop; }

void Animation::set_loop(bool l) { loop = l; }

void Animation::set_speed(double s) { speed = s; }

void Animation::set_position(double x, double y) {
    position = sf::Vector2f(x, y);

    for (auto &frame : frames) {
        frame.setPosition(position);
    }
}

void Animation::move(double x, double y) {
    position.x += x;
    position.y += y;

    for (auto &frame : frames) {
        frame.setPosition(position);
    }
}

void Animation::set_scale(double x, double y) {
    scale = sf::Vector2f(x, y);

    for (auto &frame : frames) {
        frame.setScale(scale);
    }
}

void Animation::set_origin(double x, double y) {
    origin = sf::Vector2f(x, y);

    for (auto &frame : frames) {
        frame.setOrigin(origin);
    }
}

const sf::Vector2f &Animation::get_position() const { return position; }

const sf::Vector2f &Animation::get_scale() const { return scale; }

void Animation::play() { status = Status::Playing; }
void Animation::pause() { status = Status::Paused; }
void Animation::stop() {
    status = Status::Stopped;
    current_frame = 0;
}

void Animation::render(sf::RenderWindow &window) {
    if (visible && status == Status::Playing) {
        window.draw(frames[current_frame]);
    }
}

void Animation::update(const sf::Time &delta) {
    if (status != Status::Playing)
        return;

    if ((since_update - delta).asSeconds() > (delta.asSeconds() / speed)) {
        if (current_frame >= frames_count() - 1) {
            current_frame = 0;

            if (!loop) {
                status = Status::Stopped;
            }
        } else
            current_frame++;

        since_update = sf::seconds(0);
    } else {
        since_update += delta;
    }
}

const std::size_t Animation::frames_count() const { return frames.size(); }

void Animation::init() {
    position = sf::Vector2f(0, 0);
    scale = sf::Vector2f(1, 1);
    origin = sf::Vector2f(0, 0);
    current_frame = 0;
    speed = 1.0;
    status = Status::Stopped;
    visible = true;
    loop = false;
}
}
