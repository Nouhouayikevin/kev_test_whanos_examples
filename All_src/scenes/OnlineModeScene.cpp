/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** OnlineModeScene.cpp
*/

#include "../../Include/All/Scenes/MenuScene.hpp"
#include "../../Include/All/Scenes/CreateRoomScene.hpp"
// #include "JoinRoomScene.hpp"
#include "../../Include/All/Component.hpp"
#include "../../Include/All/interfaces/ISprite.hpp"
#include "../../Include/All/Scenes/OnlineModeScene.hpp"
#include <iostream>
#include <algorithm>



// ====================================================================================
// INITIALISATION DE LA SCÈNE
// ====================================================================================
void OnlineModeScene::init(GameEngine& engine) {
    std::cout << "OnlineModeScene: Initializing..." << std::endl;
    _font_id = engine.getResourceManager().load_font("Assets/fonts/main_font.ttf");
    _bg_texture_id = engine.getResourceManager().load_texture("Assets/sprites/parallax.jpg");

    _registry.register_component<Component::Core::Position>();
    _registry.register_component<Component::Core::Velocity>();
    _registry.register_component<Component::Graphics::Sprite>();
    _registry.register_component<Component::Gameplay::ParallaxLayer>();
    _registry.register_component<Component::UI::Clickable>();
    _registry.register_component<Component::Graphics::ButtonStateComponent>();
    _registry.register_component<Component::UI::Action>();
    _registry.register_component<Component::Core::Hitbox>();
    _registry.register_component<Component::Graphics::RectangleShape>();
    _registry.register_component<Component::Graphics::UIText>();

    const float WINDOW_WIDTH = 1280.0f;
    const float WINDOW_HEIGHT = 720.0f;
    const float CENTER_X = WINDOW_WIDTH / 2.0f;
    const float CENTER_Y = WINDOW_HEIGHT / 2.0f;
    const float BUTTON_WIDTH = 350.0f;
    const float BUTTON_HEIGHT = 70.0f;

    // --- Parallax ---
    Entity bg1 = _registry.spawn_entity();
    _registry.add_component(bg1, Component::Core::Position(0, 0));
    _registry.add_component(bg1, Component::Core::Velocity(-50.0f, 0));
    _registry.add_component(bg1, Component::Graphics::Sprite(_bg_texture_id, (int)WINDOW_WIDTH, (int)WINDOW_HEIGHT, 0));
    _registry.add_component(bg1, Component::Gameplay::ParallaxLayer(-50.0f));

    Entity bg2 = _registry.spawn_entity();
    _registry.add_component(bg2, Component::Core::Position(WINDOW_WIDTH, 0));
    _registry.add_component(bg2, Component::Core::Velocity(-50.0f, 0));
    _registry.add_component(bg2, Component::Graphics::Sprite(_bg_texture_id, (int)WINDOW_WIDTH, (int)WINDOW_HEIGHT, 0));
    _registry.add_component(bg2, Component::Gameplay::ParallaxLayer(-50.0f));

    // --- Bouton de Retour ---
    {
        Entity btn = _registry.spawn_entity();
        btn.set_name("back_button");
        btn.add_component(Component::Core::Position(20, 20));
        btn.add_component(Component::Core::Hitbox(60, 60));
        btn.add_component(Component::Graphics::RectangleShape(BTN_COLOR_IDLE));
        btn.add_component(Component::Graphics::UIText("<", _font_id, 40));
        btn.add_component(Component::UI::Clickable{});
        btn.add_component(Component::Graphics::ButtonStateComponent{});
        btn.add_component(Component::UI::Action([&engine]() {
            engine.getSceneManager().add_scene("menu", std::make_shared<MenuScene>());
            engine.getSceneManager().switch_to_scene("menu", engine);
        }));
    }

    // --- Bouton CREATE ROOM ---
    {
        Entity btn = _registry.spawn_entity();
        btn.set_name("create_room_btn");
        btn.add_component(Component::Core::Position(CENTER_X - BUTTON_WIDTH / 2, CENTER_Y - 100));
        btn.add_component(Component::Core::Hitbox(BUTTON_WIDTH, BUTTON_HEIGHT));
        btn.add_component(Component::Graphics::RectangleShape(BTN_COLOR_IDLE));
        btn.add_component(Component::Graphics::UIText("CREATE.ROOM", _font_id, 35));
        btn.add_component(Component::UI::Clickable{});
        btn.add_component(Component::Graphics::ButtonStateComponent{});
        btn.add_component(Component::UI::Action([&engine]() {
            engine.getSceneManager().add_scene("create_room", std::make_shared<CreateRoomScene>());
            engine.getSceneManager().switch_to_scene("create_room", engine);
        }));
    }

    // --- Bouton JOIN ROOM ---
    {
        Entity btn = _registry.spawn_entity();
        btn.set_name("join_room_btn");
        btn.add_component(Component::Core::Position(CENTER_X - BUTTON_WIDTH / 2, CENTER_Y + 20));
        btn.add_component(Component::Core::Hitbox(BUTTON_WIDTH, BUTTON_HEIGHT));
        btn.add_component(Component::Graphics::RectangleShape(BTN_COLOR_IDLE));
        btn.add_component(Component::Graphics::UIText("JOIN ROOM", _font_id, 35));
        btn.add_component(Component::UI::Clickable{});
        btn.add_component(Component::Graphics::ButtonStateComponent{});
        btn.add_component(Component::UI::Action([&engine]() {
            std::cout << "join room clicked\n";
            // engine.getSceneManager().add_scene("join_room", std::make_shared<JoinRoomScene>());
            // engine.getSceneManager().switch_to_scene("join_room", engine);
        }));
    }

    // --- Système de clic et de survol ---
    _registry.add_system([this](GameEngine& engine, [[maybe_unused]] float dt) {
        auto& positions = _registry.get_components<Component::Core::Position>();
        auto& hitboxes = _registry.get_components<Component::Core::Hitbox>();
        auto& states = _registry.get_components<Component::Graphics::ButtonStateComponent>();
        auto& actions = _registry.get_components<Component::UI::Action>();
        auto& rects = _registry.get_components<Component::Graphics::RectangleShape>();
        
        auto mouse_pos = engine.getInputManager().getMousePosition();
        bool mouse_pressed = engine.getInputManager().isMouseButtonPressed(MouseButton::Left);
        
        for (size_t i = 0; i < states.size(); i++) {
            if (!positions[i] || !hitboxes[i] || !states[i] || !rects[i]) continue;
            
            bool is_hovered = (mouse_pos.x >= positions[i]->x && mouse_pos.x <= positions[i]->x + hitboxes[i]->width &&
                               mouse_pos.y >= positions[i]->y && mouse_pos.y <= positions[i]->y + hitboxes[i]->height);

            if (!is_hovered) {
                states[i]->state = Component::Core::ButtonState::IDLE;
                rects[i]->fill_color = BTN_COLOR_IDLE;
                continue;
            }

            states[i]->state = Component::Core::ButtonState::HOVER;
            rects[i]->fill_color = BTN_COLOR_HOVER;

            if (engine.getInputManager().isMouseButtonDown(MouseButton::Left)) {
                states[i]->state = Component::Core::ButtonState::PRESSED;
                rects[i]->fill_color = BTN_COLOR_PRESSED;
            }

            if (mouse_pressed && actions[i] && actions[i]->on_click) {
                actions[i]->on_click();
            }
        }
    });
}

// ====================================================================================
// MISE À JOUR DE LA SCÈNE
// ====================================================================================
void OnlineModeScene::update(GameEngine& engine, float delta_time) {
    auto& positions = _registry.get_components<Component::Core::Position>();
    auto& velocities = _registry.get_components<Component::Core::Velocity>();
    const float WINDOW_WIDTH = 1280.0f;

    for (size_t i = 0; i < positions.size(); i++) {
        if (positions[i] && velocities[i]) {
            positions[i]->x += velocities[i]->dx * delta_time;
            if (positions[i]->x <= -WINDOW_WIDTH) {
                positions[i]->x += WINDOW_WIDTH * 2;
            }
        }
    }
    _registry.run_systems(engine, delta_time);
}

// ====================================================================================
// RENDU DE LA SCÈNE
// ====================================================================================
void OnlineModeScene::render(GameEngine& engine) {
    IWindow& window = engine.getWindowManager();
    ResourceManager& resources = engine.getResourceManager();

    auto& positions = _registry.get_components<Component::Core::Position>();
    auto& hitboxes = _registry.get_components<Component::Core::Hitbox>();
    auto& rects = _registry.get_components<Component::Graphics::RectangleShape>();
    auto& texts = _registry.get_components<Component::Graphics::UIText>();
    auto& sprites = _registry.get_components<Component::Graphics::Sprite>();

    // --- Rendu des Sprites (Parallax) ---
    for (size_t i = 0; i < sprites.size(); i++) {
        if (sprites[i] && positions[i]) {
            ITexture& texture = resources.get_texture(sprites[i]->texture_id);
            auto sprite = resources.createSprite();
            sprite->setTexture(texture);
            sprite->setPosition({positions[i]->x, positions[i]->y});
            window.draw(*sprite);
        }
    }

    // --- Rendu des Boutons (Rectangles) ---
    for (size_t i = 0; i < rects.size(); i++) {
        if (rects[i] && positions[i] && hitboxes[i]) {
            window.drawRectangle(
                positions[i]->x, positions[i]->y,
                hitboxes[i]->width, hitboxes[i]->height,
                rects[i]->fill_color
            );
        }
    }

    // --- Rendu des Textes ---
    for (size_t i = 0; i < texts.size(); i++) {
        if (texts[i] && positions[i]) {
            IFont& font = resources.get_font(texts[i]->font_id);
            // Calcul simplifié pour centrer le texte
            float text_width = texts[i]->content.length() * texts[i]->character_size * 0.5f;
            float x_offset = 0;
            float y_offset = 0;

            if(hitboxes[i]) { // Si le texte est sur un bouton, on le centre
                 x_offset = (hitboxes[i]->width - text_width) / 2.0f;
                 y_offset = (hitboxes[i]->height - texts[i]->character_size) / 2.0f;
            }

            if (texts[i]->content == "<") {
                x_offset -= 10; 
            } else if (texts[i]->content == "CREATE.ROOM") {
                x_offset -= 72;
            } else {
                x_offset -= 60;
            }

            window.drawText(
                texts[i]->content,
                positions[i]->x + x_offset,
                positions[i]->y + y_offset,
                font,
                texts[i]->character_size,
                texts[i]->color
            );
        }
    }
}