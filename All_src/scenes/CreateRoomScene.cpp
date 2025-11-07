/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** CreateRoomScene.cpp
*/

#include "../../Include/All/Scenes/CreateRoomScene.hpp"
#include "../../Include/All/Scenes/OnlineModeScene.hpp"
#include "../../Include/All/Component.hpp"
#include "../../Include/All/interfaces/ISprite.hpp"
#include <iostream>
#include <algorithm>
#include "../../Include/All/managers/SceneManager.hpp" 
void CreateRoomScene::init(GameEngine& engine) {
    std::cout << "CreateRoomScene: Initializing..." << std::endl;
    _font_id = engine.getResourceManager().load_font("Assets/fonts/main_font.ttf");
    _bg_texture_id = engine.getResourceManager().load_texture("Assets/sprites/parallax.jpg");

    _registry.register_component<Component::Core::Position>();
    _registry.register_component<Component::Core::Velocity>();
    _registry.register_component<Component::Graphics::Sprite>();
    _registry.register_component<Component::UI::Clickable>();
    _registry.register_component<Component::Graphics::ButtonStateComponent>();
    _registry.register_component<Component::UI::Action>();
    _registry.register_component<Component::Core::Hitbox>();
    _registry.register_component<Component::Graphics::RectangleShape>();
    _registry.register_component<Component::Graphics::UIText>();
    _registry.register_component<Component::UI::TextInput>();

    const float WINDOW_WIDTH = 1280.0f;
    const float WINDOW_HEIGHT = 720.0f;
    const float CENTER_X = WINDOW_WIDTH / 2.0f;
    const float FIELD_WIDTH = 550.0f; // CORRECTION : Augmentation de la largeur
    const float FIELD_HEIGHT = 60.0f;
    float y_pos = 150.0f;

    Entity bg1 = _registry.spawn_entity();
    _registry.add_component(bg1, Component::Core::Position(0, 0));
    _registry.add_component(bg1, Component::Core::Velocity(-50.0f, 0));
    _registry.add_component(bg1, Component::Graphics::Sprite(_bg_texture_id, (int)WINDOW_WIDTH, (int)WINDOW_HEIGHT, 0));

    Entity bg2 = _registry.spawn_entity();
    _registry.add_component(bg2, Component::Core::Position(WINDOW_WIDTH, 0));
    _registry.add_component(bg2, Component::Core::Velocity(-50.0f, 0));
    _registry.add_component(bg2, Component::Graphics::Sprite(_bg_texture_id, (int)WINDOW_WIDTH, (int)WINDOW_HEIGHT, 0));

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
            engine.getSceneManager().add_scene("online_mode", std::make_shared<OnlineModeScene>());
            engine.getSceneManager().switch_to_scene("online_mode", engine);
        }));
    }

    {
        Entity field = _registry.spawn_entity();
        field.set_name("pseudo_input");
        field.add_component(Component::Core::Position(CENTER_X - FIELD_WIDTH / 2, y_pos));
        field.add_component(Component::Core::Hitbox(FIELD_WIDTH, FIELD_HEIGHT));
        field.add_component(Component::Graphics::RectangleShape(FIELD_COLOR_NORMAL));
        field.add_component(Component::Graphics::UIText("Pseudo", _font_id, 30));
        field.add_component(Component::UI::Clickable{});
        field.add_component(Component::UI::TextInput("Pseudo"));
        y_pos += 100;
    }

    {
        Entity field = _registry.spawn_entity();
        field.set_name("players_input");
        field.add_component(Component::Core::Position(CENTER_X - FIELD_WIDTH / 2, y_pos));
        field.add_component(Component::Core::Hitbox(FIELD_WIDTH, FIELD_HEIGHT));
        field.add_component(Component::Graphics::RectangleShape(FIELD_COLOR_NORMAL));
        field.add_component(Component::Graphics::UIText("Nombre de joueurs", _font_id, 30));
        field.add_component(Component::UI::Clickable{});
        field.add_component(Component::UI::TextInput("Nombre de joueurs"));
        y_pos += 100;
    }

    {
        Entity field = _registry.spawn_entity();
        field.set_name("password_input");
        field.add_component(Component::Core::Position(CENTER_X - FIELD_WIDTH / 2, y_pos));
        field.add_component(Component::Core::Hitbox(FIELD_WIDTH, FIELD_HEIGHT));
        field.add_component(Component::Graphics::RectangleShape(FIELD_COLOR_NORMAL));
        field.add_component(Component::Graphics::UIText("Mot de passe (optionnel)", _font_id, 30));
        field.add_component(Component::UI::Clickable{});
        field.add_component(Component::UI::TextInput("Mot de passe (optionnel)"));
        y_pos += 120;
    }

    {
        Entity btn = _registry.spawn_entity();
        btn.set_name("submit_create_btn");
        btn.add_component(Component::Core::Position(CENTER_X - FIELD_WIDTH / 2, y_pos));
        btn.add_component(Component::Core::Hitbox(FIELD_WIDTH, FIELD_HEIGHT));
        btn.add_component(Component::Graphics::RectangleShape(BTN_COLOR_IDLE));
        btn.add_component(Component::Graphics::UIText("CRÉER", _font_id, 35));
        btn.add_component(Component::UI::Clickable{});
        btn.add_component(Component::Graphics::ButtonStateComponent{});
        btn.add_component(Component::UI::Action([this]() {
            auto& pseudo_input = _registry.get_entity_by_name("pseudo_input").value().get_component<Component::UI::TextInput>();
            auto& players_input = _registry.get_entity_by_name("players_input").value().get_component<Component::UI::TextInput>();
            auto& password_input = _registry.get_entity_by_name("password_input").value().get_component<Component::UI::TextInput>();
            std::cout << "Création de la salle avec: Pseudo=" << pseudo_input.buffer 
                      << ", Joueurs=" << players_input.buffer 
                      << ", Mot de passe=" << password_input.buffer << std::endl;
        }));
    }

    _registry.add_system([this](GameEngine& engine, [[maybe_unused]] float dt) {
        auto& positions = _registry.get_components<Component::Core::Position>();
        auto& hitboxes = _registry.get_components<Component::Core::Hitbox>();
        auto& states = _registry.get_components<Component::Graphics::ButtonStateComponent>();
        auto& actions = _registry.get_components<Component::UI::Action>();
        auto& rects = _registry.get_components<Component::Graphics::RectangleShape>();
        auto& text_inputs = _registry.get_components<Component::UI::TextInput>();
        
        auto mouse_pos = engine.getInputManager().getMousePosition();
        bool mouse_pressed = engine.getInputManager().isMouseButtonPressed(MouseButton::Left);
        
        for (size_t i = 0; i < positions.size(); i++) {
            if (!positions[i] || !hitboxes[i]) continue;
            
            bool is_hovered = (mouse_pos.x >= positions[i]->x && mouse_pos.x <= positions[i]->x + hitboxes[i]->width &&
                               mouse_pos.y >= positions[i]->y && mouse_pos.y <= positions[i]->y + hitboxes[i]->height);

            if (text_inputs[i]) {
                if (mouse_pressed && is_hovered) {
                    for(auto& other_input : text_inputs) { if(other_input) other_input->is_focused = false; }
                    text_inputs[i]->is_focused = true;
                }
                rects[i]->fill_color = text_inputs[i]->is_focused ? FIELD_COLOR_FOCUSED : FIELD_COLOR_NORMAL;
            }

            if (states[i]) {
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
        }
    });
    
    _registry.add_system([this](GameEngine& engine, [[maybe_unused]] float dt) {
        auto& text_inputs = _registry.get_components<Component::UI::TextInput>();
        auto& ui_texts = _registry.get_components<Component::Graphics::UIText>();
        IInputManager& input = engine.getInputManager();
        
        for(size_t i = 0; i < text_inputs.size(); ++i) {
            if(!text_inputs[i] || !ui_texts[i]) continue;

            if (text_inputs[i]->is_focused) {
                // CORRECTION : Gestion du backspace
                if (input.isKeyPressed(KeyCode::R) && !text_inputs[i]->buffer.empty()) {
                    text_inputs[i]->buffer.pop_back();
                }
                // Gestion des lettres
                for (int k = (int)KeyCode::A; k <= (int)KeyCode::Z; ++k) {
                    if (input.isKeyPressed((KeyCode)k)) {
                        if (text_inputs[i]->buffer.length() < text_inputs[i]->max_chars) {
                            text_inputs[i]->buffer += (char)('a' + (k - (int)KeyCode::A));
                        }
                    }
                }
            }

            if (text_inputs[i]->buffer.empty() && !text_inputs[i]->is_focused) {
                ui_texts[i]->content = text_inputs[i]->placeholder;
            } else {
                std::string cursor = (text_inputs[i]->is_focused && (int)(engine.getClock().getElapsedTime() * 2.0f) % 2 == 0) ? "_" : "";
                ui_texts[i]->content = text_inputs[i]->buffer + cursor;
            }
        }
    });
}

void CreateRoomScene::update(GameEngine& engine, float delta_time) {
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

void CreateRoomScene::render(GameEngine& engine) {
    IWindow& window = engine.getWindowManager();
    ResourceManager& resources = engine.getResourceManager();

    auto& positions = _registry.get_components<Component::Core::Position>();
    auto& hitboxes = _registry.get_components<Component::Core::Hitbox>();
    auto& rects = _registry.get_components<Component::Graphics::RectangleShape>();
    auto& texts = _registry.get_components<Component::Graphics::UIText>();
    auto& sprites = _registry.get_components<Component::Graphics::Sprite>();

    for (size_t i = 0; i < sprites.size(); i++) {
        if (sprites[i] && positions[i]) {
            ITexture& texture = resources.get_texture(sprites[i]->texture_id);
            auto sprite = resources.createSprite();
            sprite->setTexture(texture);
            sprite->setPosition({positions[i]->x, positions[i]->y});
            window.draw(*sprite);
        }
    }
    for (size_t i = 0; i < rects.size(); i++) {
        if (rects[i] && positions[i] && hitboxes[i]) {
            window.drawRectangle(positions[i]->x, positions[i]->y, hitboxes[i]->width, hitboxes[i]->height, rects[i]->fill_color);
        }
    }
    for (size_t i = 0; i < texts.size(); i++) {
        if (texts[i] && positions[i]) {
            IFont& font = resources.get_font(texts[i]->font_id);
            float x_offset = 20; 
            float y_offset = (hitboxes[i] ? (hitboxes[i]->height - texts[i]->character_size) / 2.0f : 0);
            window.drawText(texts[i]->content, positions[i]->x + x_offset, positions[i]->y + y_offset, font, texts[i]->character_size, texts[i]->color);
        }
    }
}