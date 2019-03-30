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

    loader
        ->add_music("music_1", "../tests/assets/click.ogg")
        ->add_music("music_2", "../tests/assets/click.ogg");
    loader->load_music();

    ASSERT_EQ(loader->get_music("music_1")->getStatus(), sf::SoundSource::Status::Stopped);
    ASSERT_EQ(loader->get_music("music_2")->getStatus(), sf::SoundSource::Status::Stopped);

    ASSERT_THROW(loader->get_music("doesnt_exist"), std::out_of_range);
}

TEST(TestLoader, test_load_sounds) {
    auto loader = std::make_unique<K::Loader>();

    loader
        ->add_sound_buffer("sound_1", "../tests/assets/click.ogg")
        ->add_sound_buffer("sound_2", "../tests/assets/click.ogg");
    loader->load_sound_buffers();

    ASSERT_EQ(loader->get_sound("sound_1")->getStatus(), sf::SoundSource::Status::Stopped);
    ASSERT_EQ(loader->get_sound("sound_2")->getStatus(), sf::SoundSource::Status::Stopped);

    // Insert additional sound with the same sound buffer as sound_1
    loader->insert_sound("sound_1", "other_sound_1");
    loader->get_sound("other_sound_1")->setVolume(0);

    ASSERT_EQ(loader->get_sound("other_sound_1")->getStatus(), sf::SoundSource::Status::Stopped);

    // Remains unchanged
    ASSERT_EQ(loader->get_sound("sound_1")->getVolume(), 100);

    ASSERT_THROW(loader->get_music("doesnt_exist"), std::out_of_range);
}

TEST(TestLoader, test_load_fonts) {
    auto loader = std::make_unique<K::Loader>();

    loader
        ->add_font("font_1", "../tests/assets/DejaVuSans.ttf")
        ->add_font("font_2", "../tests/assets/DejaVuSans.ttf");
    loader->load_fonts();

    ASSERT_EQ(loader->get_font("font_1").getInfo().family, "DejaVu Sans");
    ASSERT_EQ(loader->get_font("font_2").getInfo().family, "DejaVu Sans");

    ASSERT_THROW(loader->get_font("doesnt_exist"), std::out_of_range);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
