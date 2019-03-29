#include <gtest/gtest.h>
#include <memory>

#include "../src/loader.hpp"

TEST(TestLoader, test_load_textures) {
    auto loader = std::make_unique<K::Loader>();

    loader
        ->add_texture("image_1", "../tests/assets/image_1.png")
        ->add_texture("image_2", "../tests/assets/image_2.png");

    loader->load_textures();

    ASSERT_EQ(loader->get_texture("image_1").getSize(), sf::Vector2u(32, 32));
    ASSERT_EQ(loader->get_texture("image_2").getSize(), sf::Vector2u(32, 32));

    ASSERT_THROW(loader->get_texture("doesnt_exist"), std::out_of_range);
}

TEST(TestLoader, test_load_music) {
    auto loader = std::make_unique<K::Loader>();

    loader->add_music("music_1", "../tests/assets/click.ogg");
    loader->load_music();

    ASSERT_EQ(loader->get_music("music_1")->getStatus(), sf::SoundSource::Status::Stopped);

    ASSERT_THROW(loader->get_music("doesnt_exist"), std::out_of_range);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
