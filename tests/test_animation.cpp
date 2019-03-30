#include <memory>
#include <vector>

#include <gtest/gtest.h>

#include "../src/loader.hpp"
#include "../src/animation.hpp"

TEST(TestAnimation, test_create_animation_from_textures) {
    auto loader = std::make_unique<K::Loader>();

    loader
        ->add_texture("image_1", "../tests/assets/image_1.png")
        ->add_texture("image_2", "../tests/assets/image_2.png");
    loader->load();

    std::vector<const sf::Texture*> v{loader->get_texture("image_1"), loader->get_texture("image_2")};
    K::Animation animation(v);

    ASSERT_EQ(animation.frames_count(), 2);
    ASSERT_EQ(animation.get_status(), K::Animation::Status::Stopped);
    ASSERT_TRUE(animation.is_visible());
    ASSERT_FALSE(animation.is_looping());
}

TEST(TestAnimation, test_create_animation_with_loader) {
    auto loader = std::make_unique<K::Loader>();

    loader
        ->add_texture("image_1", "../tests/assets/image_1.png")
        ->add_texture("image_2", "../tests/assets/image_2.png");
    loader->load();

    K::Animation animation(*loader, std::vector<std::string>{"image_1", "image_2"});

    ASSERT_EQ(animation.frames_count(), 2);
    ASSERT_EQ(animation.get_status(), K::Animation::Status::Stopped);
    ASSERT_TRUE(animation.is_visible());
    ASSERT_FALSE(animation.is_looping());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
