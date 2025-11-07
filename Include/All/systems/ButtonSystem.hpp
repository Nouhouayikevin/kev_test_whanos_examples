/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** ButtonSystem.hpp
*/

#pragma once

#include "../interfaces/ISystem.hpp"
#include "../Component.hpp"
#include "../../GameEngine_Include/core/GameEngine.hpp"
#include <string>

class ButtonSystem : public ISystem {
private:
    std::string _registerGroup;

public:
    ButtonSystem(std::string registerGroup = "default") 
        : _registerGroup(registerGroup) {}

    void run(GameEngine& gameEng, float dt) override {
        Registry& registry = gameEng.getRegistry(_registerGroup);
        IInputManager& input = gameEng.getInputManager();
        
        auto& positions = registry.get_components<Component::Core::Position>();
        auto& hitboxes = registry.get_components<Component::Core::Hitbox>();
        auto& states = registry.get_components<Component::Graphics::ButtonStateComponent>();
        auto& actions = registry.get_components<Component::UI::Action>();
        auto& clickables = registry.get_components<Component::UI::Clickable>();
        
        auto mouse_pos = input.getMousePosition();
        
        size_t entity_count = registry.get_entity_count();
        
        for (size_t i = 0; i < entity_count; ++i) {
            if (!positions[i] || !hitboxes[i] || !states[i] || !clickables[i]) 
                continue;
            
            auto& pos = *positions[i];
            auto& hitbox = *hitboxes[i];
            auto& state_comp = *states[i];
            auto current_state = state_comp.state;

            // Check if mouse is hovering over button
            bool is_hovered = (mouse_pos.x >= pos.x && 
                             mouse_pos.x <= pos.x + hitbox.width &&
                             mouse_pos.y >= pos.y && 
                             mouse_pos.y <= pos.y + hitbox.height);

            if (!is_hovered) {
                state_comp.state = Component::Core::ButtonState::IDLE;
                continue;
            }

            // Transition from IDLE to HOVER
            if (current_state == Component::Core::ButtonState::IDLE) {
                state_comp.state = Component::Core::ButtonState::HOVER;
            }

            // Detect button press
            if (input.isMouseButtonPressed(MouseButton::Left)) {
                state_comp.state = Component::Core::ButtonState::PRESSED;
            }

            // Fire action on release (while still hovering)
            if (state_comp.state == Component::Core::ButtonState::PRESSED && 
                !input.isMouseButtonDown(MouseButton::Left)) {
                
                if (actions[i] && actions[i]->on_click) {
                    actions[i]->on_click();
                }
                
                state_comp.state = Component::Core::ButtonState::HOVER;
            }
        }
    }
};
