#include <iostream>

#include <SFML/Graphics.hpp>

#include "../src/animation.hpp"
#include "../src/loader.hpp"

int main(int argc, char **argv) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Animation example");

    auto loader = std::make_unique<K::Loader>();

    loader->add_texture("image_1", "../tests/assets/image_1.png")
        ->add_texture("image_2", "../tests/assets/image_2.png");
    loader->load();

    K::Animation animation(*loader,
                           std::vector<std::string>{"image_1", "image_2"});
    animation.set_speed(0.1);
    animation.set_loop(true);
    animation.play();

    sf::Time per_frame = sf::seconds(1.0f / 60.0f);
    sf::Clock clock;
    sf::Time last_update;

    while (window.isOpen()) {
        sf::Event event;

        sf::Time delta = clock.getElapsedTime() - last_update;

        if (delta > per_frame) {
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
                }
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            animation.update(delta);

            window.clear();
            animation.render(window);
            window.display();

            last_update = clock.getElapsedTime();
        }
    }

    return 0;
}
