/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** WinScene.cpp
*/

#include "../../Include/All/Scenes/WinScene.hpp"
#include "../../Include/All/interfaces/ISprite.hpp"
#include <iostream>
#include <algorithm>

const std::string WIN_BG_PATH = "Assets/sprites/parallax.jpg";
static size_t bg_texture_id;
static size_t font_id;

void WinScene::init(GameEngine& engine)
{
    std::cout << "WinScene: Initializing..." << std::endl;

    bg_texture_id = engine.getResourceManager().load_texture(WIN_BG_PATH);
    font_id = engine.getResourceManager().load_font("Assets/fonts/main_font.ttf");

    _registry.register_component<Component::Core::Position>();
    _registry.register_component<Component::Core::Velocity>();
    _registry.register_component<Component::Graphics::Sprite>();
    _registry.register_component<Component::Graphics::UIText>();
    _registry.register_component<Component::Gameplay::ParallaxLayer>();

    const float WINDOW_WIDTH = 1280.0f;
    const float WINDOW_HEIGHT = 720.0f;
    const float CENTER_X = WINDOW_WIDTH / 2.0f;

    Entity bg1 = _registry.spawn_entity();
    _registry.add_component(bg1, Component::Core::Position(0, 0));
    _registry.add_component(bg1, Component::Core::Velocity(-40.0f, 0));
    _registry.add_component(bg1, Component::Graphics::Sprite(bg_texture_id, WINDOW_WIDTH, WINDOW_HEIGHT, 0));
    _registry.add_component(bg1, Component::Gameplay::ParallaxLayer(-40.0f));

    Entity bg2 = _registry.spawn_entity();
    _registry.add_component(bg2, Component::Core::Position(WINDOW_WIDTH, 0));
    _registry.add_component(bg2, Component::Core::Velocity(-40.0f, 0));
    _registry.add_component(bg2, Component::Graphics::Sprite(bg_texture_id, WINDOW_WIDTH, WINDOW_HEIGHT, 0));
    _registry.add_component(bg2, Component::Gameplay::ParallaxLayer(-40.0f));

    float victory_score = 0;
    float high_score = 0;

    Entity title = _registry.spawn_entity();
    title.set_name("RTypeTitle");
    title.add_component<Component::Core::Position>(
        Component::Core::Position(CENTER_X - 150, 100));
    title.add_component<Component::Graphics::UIText>(
        Component::Graphics::UIText("R-TYPE", font_id, 70, Component::Core::Color(255, 255, 255)));

    Entity victory_label = _registry.spawn_entity();
    victory_label.set_name("VictoryLabel");
    victory_label.add_component<Component::Core::Position>(
        Component::Core::Position(CENTER_X - 220, WINDOW_HEIGHT / 2.5f));
    victory_label.add_component<Component::Graphics::UIText>(
        Component::Graphics::UIText("VICTORY :", font_id, 60, Component::Core::Color(255, 255, 255)));

    Entity victory_value = _registry.spawn_entity();
    victory_value.set_name("VictoryValue");
    victory_value.add_component<Component::Core::Position>(
        Component::Core::Position(CENTER_X + 250, WINDOW_HEIGHT / 2.5f));
    victory_value.add_component<Component::Graphics::UIText>(
        Component::Graphics::UIText(std::to_string(static_cast<int>(victory_score)), font_id, 60, Component::Core::Color(255, 215, 0)));

    Entity high_label = _registry.spawn_entity();
    high_label.set_name("HighScoreLabel");
    high_label.add_component<Component::Core::Position>(
        Component::Core::Position(CENTER_X - 250, WINDOW_HEIGHT - 150));
    high_label.add_component<Component::Graphics::UIText>(
        Component::Graphics::UIText("HIGH SCORE :", font_id, 50, Component::Core::Color(255, 255, 255)));

    Entity high_value = _registry.spawn_entity();
    high_value.set_name("HighScoreValue");
    high_value.add_component<Component::Core::Position>(
        Component::Core::Position(CENTER_X + 280, WINDOW_HEIGHT - 150));
    high_value.add_component<Component::Graphics::UIText>(
        Component::Graphics::UIText(std::to_string(static_cast<int>(high_score)), font_id, 50, Component::Core::Color(0, 255, 0)));

    std::cout << "WinScene initialized for window " << WINDOW_WIDTH << "x" << WINDOW_HEIGHT << std::endl;
}

void WinScene::update(GameEngine& engine, float delta_time)
{
    auto& positions = _registry.get_components<Component::Core::Position>();
    auto& velocities = _registry.get_components<Component::Core::Velocity>();
    const float WINDOW_WIDTH = 1280.0f;

    for (size_t i = 0; i < positions.size(); i++) {
        if (positions[i] && velocities[i]) {
            positions[i]->x += velocities[i]->dx * delta_time;
            if (positions[i]->x <= -WINDOW_WIDTH)
                positions[i]->x = WINDOW_WIDTH - 1;
        }
    }

    _registry.run_systems(engine, delta_time);
}

void WinScene::render(GameEngine& engine)
{
    IWindow& window = engine.getWindowManager();
    ResourceManager& resources = engine.getResourceManager();

    auto& positions = _registry.get_components<Component::Core::Position>();
    auto& texts = _registry.get_components<Component::Graphics::UIText>();
    auto& sprites = _registry.get_components<Component::Graphics::Sprite>();

    for (size_t i = 0; i < sprites.size() && i < positions.size(); i++) {
        if (sprites[i] && positions[i]) {
            ITexture& texture = resources.get_texture(sprites[i]->texture_id);
            auto sprite = resources.createSprite();
            sprite->setTexture(texture);
            sprite->setPosition({positions[i]->x, positions[i]->y});
            sprite->setTextureRect({
                sprites[i]->source_rect.x,
                sprites[i]->source_rect.y,
                sprites[i]->source_rect.width,
                sprites[i]->source_rect.height
            });
            window.draw(*sprite);
        }
    }

    for (size_t i = 0; i < texts.size(); i++) {
        if (texts[i] && positions[i]) {
            IFont& font = resources.get_font(texts[i]->font_id);
            window.drawText(
                texts[i]->content,
                positions[i]->x,
                positions[i]->y,
                font,
                texts[i]->character_size,
                texts[i]->color
            );
        }
    }
}
