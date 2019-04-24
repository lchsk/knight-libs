#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>

#include <limits>
#include <map>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>

#include <SFML/Graphics.hpp>

#define KNIGHT_DEBUG

#include "../src/simple_ecs.hpp"
#include "../src/animation.hpp"
#include "../src/loader.hpp"
#include "../src/hex.hpp"
#include "../src/hex_map.hpp"
#include "../src/game.hpp"
#include "../src/pathfinding.hpp"
#include "../src/gui/gui.hpp"

struct Animated {
    std::vector<K::Animation> animations;
};

struct AnimatedObject {
    std::unordered_map<std::string, K::Animation> animations;

    std::string active_animation;
    std::string direction;
    K::Hex position;

    void set_position(const K::Hex& hex, const K::Point& hex_size, K::HexMap<Animated>* hex_map) {
        position = hex;

        auto& anim = animations[active_animation];
        const auto sprite_size = anim.get_sprite_size();

        const auto p = hex_map->get_hex_middle(hex);

        anim.set_position(p.x - sprite_size.x / 2, p.y - sprite_size.y / 2);
    }

    void update(const sf::Time &delta, const sf::Vector2i &mouse_pos) {
        animations[active_animation].update(delta);
    }

    void render(sf::RenderWindow &window) {
        animations[active_animation].render(window);
    }
};

ecs::Entity* build_unit(K::Loader& loader, ecs::System& system, K::HexMap<Animated>* hex_map) {
    auto entity = system.create();

    system.add<AnimatedObject>(entity);

    K::Animation anim(loader, std::vector<std::string>{
            "test-7.png",
                "test-8.png",
                "test-9.png",
                "test-10.png",
                }, sf::Rect(0, 192, 64, 64));

    auto& a = entity->get<AnimatedObject>();

    a->animations["attack"] = anim;
    a->active_animation = "attack";
    a->set_position(K::Hex(0, 0, 0), hex_map->get_hex_size(), hex_map);

    return entity;
}

int main(int argc, char **argv) {
    K::Game game;

    game.window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "knights");

    game.get_window().setMouseCursorVisible(false);
    // window.setFramerateLimit(60);

    // auto loader = std::make_unique<K::Loader>();
    game.loader = std::make_unique<K::Loader>();

    game.loader->add_texture("hex_1", "../tests/assets/grass_05.png")
        ->add_texture("hex_2", "../tests/assets/dirt_06.png")
        ->add_texture("brickBuilding1", "../tests/assets/brickBuilding1.png")
        ->add_texture("tank", "../tests/assets/tank.png")
        ->add_texture("treeRound_large.png", "../tests/assets/treeRound_large.png")

        ->add_texture("test-7.png", "../tests/assets/test-7.png")
        ->add_texture("test-8.png", "../tests/assets/test-8.png")
        ->add_texture("test-9.png", "../tests/assets/test-9.png")
        ->add_texture("test-10.png", "../tests/assets/test-10.png")
        ->add_texture("test-11.png", "../tests/assets/test-11.png")
        ->add_texture("test-12.png", "../tests/assets/test-12.png")

        ->add_font("font_1", "../tests/assets/DejaVuSans.ttf");
    game.loader->load();

    

    std::unordered_map<K::Hex, std::string> m1 = {
        {K::Hex(0, 0, 0), "hex_1"},
        {K::Hex(1, 0, -1), "hex_2"},
        {K::Hex(-1, 1, 0), "hex_1"},
        {K::Hex(0, 1, -1), "hex_2"},
    };


    std::unordered_map<K::Hex, std::vector<std::string>> m2 = {
        {K::Hex(0, 0, 0), {"hex_1"}},
        {K::Hex(1, 0, -1), {"hex_2", "brickBuilding1"}},
        {K::Hex(-1, 1, 0), {"hex_1"}},

        {K::Hex(0, 1, -1), {"hex_2"}},
        {K::Hex(1, 1, -2), {"hex_2"}},
        {K::Hex(2, 1, -3), {"hex_2"}},
        {K::Hex(3, 1, -4), {"hex_2"}},
        {K::Hex(4, 1, -5), {"hex_2"}},
        {K::Hex(5, 1, -6), {"hex_2"}},
        {K::Hex(6, 1, -7), {"hex_2"}},
        {K::Hex(7, 1, -8), {"hex_2"}},
        {K::Hex(8, 1, -9), {"hex_2"}},
        {K::Hex(9, 1, -10), {"hex_2"}},
        {K::Hex(10, 1, -11), {"hex_2"}},
        {K::Hex(11, 1, -12), {"hex_2"}},

        {K::Hex(2, 0, -2), {"hex_2"}},

        {K::Hex(1, 2, -3), {"hex_2"}},
        {K::Hex(2, 2, -4), {"hex_2"}},
        {K::Hex(2, 3, -5), {"hex_2"}},
        {K::Hex(3, 3, -6), {"hex_2"}},
        {K::Hex(4, 4, -8), {"hex_2"}},
        {K::Hex(5, 5, -10), {"hex_2"}},
        {K::Hex(6, 6, -12), {"hex_2"}},
        {K::Hex(7, 7, -14), {"hex_1"}},
    };

    K::PathFinder path_finder;

    path_finder.nodes[K::Hex(0, 0, 0)].push_back(K::MapNode(K::Hex(1, 0, -1), 1));
    path_finder.nodes[K::Hex(0, 0, 0)].push_back(K::MapNode(K::Hex(0, 1, -1), 0.5));
    path_finder.nodes[K::Hex(0, 0, 0)].push_back(K::MapNode(K::Hex(-1, 1, 0), 1));

    path_finder.nodes[K::Hex(1, 0, -1)].push_back(K::MapNode(K::Hex(2, 0, -2), 1));
    path_finder.nodes[K::Hex(1, 0, -1)].push_back(K::MapNode(K::Hex(1, 1, -2), 1));
    path_finder.nodes[K::Hex(1, 0, -1)].push_back(K::MapNode(K::Hex(0, 1, -1), 1));

    path_finder.nodes[K::Hex(0, 1, -1)].push_back(K::MapNode(K::Hex(1, 1, -2), 1));
    path_finder.nodes[K::Hex(0, 1, -1)].push_back(K::MapNode(K::Hex(1, 0, -1), 1));

    path_finder.find_paths(K::Hex(0, 0, 0));

    std::cout << "Distance from 0 to 4: " << path_finder.get_min_distance(K::Hex(1, 1, -2)) << std::endl;

    auto path = path_finder.get_shortest_path_to(K::Hex(1, 1, -2));

    std::cout << "Path : " << path.size() << std::endl;

    for (auto h : path) {
        std::cout << h.to_str();
        std::cout << std::endl;
    }
    std::cout << std::endl;

    K::Gui gui;
    gui.grid.init_grid(K::Point(800, 600), K::Point(16, 12));

    K::Animation anim_btn1(game.get_loader(), std::vector<std::string>{"test-7.png"});
    K::Animation anim_btn2(game.get_loader(), std::vector<std::string>{"hex_1"});
    K::Animation anim_btn3(game.get_loader(), std::vector<std::string>{"brickBuilding1"});

    gui.create_button("cursor");
    auto c = gui.get_button("cursor");
    c->animations["default"] = anim_btn3;
    c->clickable = false;

    gui.create_button("label");
    auto l = gui.get_button("label");
    l->clickable = false;
    l->rollover_time = 0.7;
    l->enqueue_message("a");
    l->enqueue_message("b");
    l->enqueue_message("c");
    l->enqueue_message("d");
    l->enqueue_message("e");
    gui.set_button_style("label", game.get_loader(), "font_1", 18);
    l->set_text("test\nnext line");
    l->set_position(gui.grid.get_position(K::Point(10, 1)));

    gui.create_button("btn");

    auto b = gui.get_button("btn");

    b->animations["default"] = anim_btn2;
    b->animations["pressed"] = anim_btn2;
    b->animations["hover"] = anim_btn3;

    gui.set_button_style("btn", game.get_loader(), "font_1", 14);
    b->set_text("test");

    b->set_position(gui.grid.get_position(K::Point(6, 0)));

    b->pressed_size = 20;
    b->pressed_color = sf::Color::Red;

    b->hover_size = 16;
    b->hover_color = sf::Color::Yellow;

    K::Layout layout(K::Hex::ORIENTATION_POINTY, K::Point(69, 69), K::Point(0, 0));

    const K::Point hex_size(120, 140);

    const auto p1 = K::Hex::from_point(layout, hex_size);

    const K::Point origin(0, 0);
    const K::Point size(800, 600);

    sf::View main_view(sf::FloatRect(origin.x, origin.y, size.x, size.y));
    sf::View minimap_view(sf::FloatRect(origin.x, origin.y, size.x, size.y));
    sf::View gui_view(sf::FloatRect(origin.x, origin.y, size.x, size.y));

    game.set_main_view(std::move(main_view));
    game.set_minimap_view(std::move(minimap_view));
    game.set_gui_view(std::move(gui_view));

    K::HexCamera hex_camera(origin, size, hex_size, game.get_main_view());
    hex_camera.set_speed(5);
    hex_camera.set_window_pct(0.18);

    auto hex_map = std::make_unique<K::HexMap<Animated>>(layout, hex_camera, hex_size, game.get_minimap_view());

    game.get_minimap_view()->zoom(3);
    game.get_minimap_view()->setViewport(sf::FloatRect(0.75f, 0.f, 0.25f, 0.25f));

    hex_map->set_hexes(game.get_loader(), m2);
    hex_map->set_view(game.get_window());
    hex_map->set_font(game.get_loader().get_font("font_1"));

    sf::Time per_frame = sf::seconds(1.0f / 60.0f);
    sf::Clock clock;
    sf::Time last_update;

    ecs::System system;

    auto entity = build_unit(game.get_loader(), system, hex_map.get());

    while (game.get_window().isOpen()) {
        sf::Event event;

        sf::Time delta = clock.getElapsedTime() - last_update;

        sf::Vector2i pos;
        pos = sf::Mouse::getPosition(game.get_window());
        auto world_pos = game.get_window().mapPixelToCoords(pos);

        while (game.get_window().pollEvent(event)) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        game.get_window().close();
                    }
                }
                if (event.type == sf::Event::Closed)
                    game.get_window().close();
            }

        if (delta > per_frame) {

            const auto view = ecs::VectorView::create<AnimatedObject>(system);

            hex_map->update(delta, pos);

            for (ecs::Entity *e : view.entities()) {
                auto &ao = e->get<AnimatedObject>();
                ao->update(delta, pos);
            }

            gui.update(delta, static_cast<sf::Vector2f>(pos), event);

            auto b = gui.get_button("btn");

            if (b->mouse_pressed) {

            }
            if (b->mouse_released) {

            }

            auto c = gui.get_button("cursor");
            c->set_position(K::Point(pos.x, pos.y));

            game.get_window().clear();
            game.get_window().setView(*game.get_main_view());
            hex_map->render(*game.window);

            for (ecs::Entity *e : view.entities()) {
                auto &ao = e->get<AnimatedObject>();
                ao->render(game.get_window());
            }

            game.get_window().setView(*game.get_minimap_view());
            hex_map->draw_tiles(game.get_window());

            game.get_window().setView(*game.get_gui_view());
            gui.render(game.get_window());

            game.get_window().display();

            last_update = clock.getElapsedTime();
        }
    }

    return 0;
}
