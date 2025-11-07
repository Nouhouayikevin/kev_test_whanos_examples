/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** SfmlInputManager.hpp
*/

// =================================================================
//  FICHIER: sfml/SfmlInputManager.hpp
//  RÔLE:    Définit l'implémentation SFML de IInputManager.
// =================================================================

#pragma once

#include "../interfaces/IInputManager.hpp"
#include "../../GameEngine_Include/core/Event.hpp"
#include <map>
#include <vector>

class SfmlWindow;

class SfmlInputManager : public IInputManager {
public:
    explicit SfmlInputManager(SfmlWindow& window);

    void update() override;
    bool isKeyDown(KeyCode key) const override;
    bool isKeyPressed(KeyCode key) const override;

    bool isMouseButtonDown(MouseButton button) const override;
    bool isMouseButtonPressed(MouseButton button) const override;
    Vector2i getMousePosition() const override;
    bool isWindowClosed() const override;
    bool isKeyReleased(KeyCode key) const override;
    bool isMouseButtonReleased(MouseButton button) const override;

private:
    SfmlWindow& _window;

    std::map<MouseButton, bool> _mouse_button_states;
    std::map<MouseButton, bool> _previous_mouse_button_states;
    std::map<KeyCode, bool> _key_states;
    std::map<KeyCode, bool> _previous_key_states;
    Vector2i _mouse_position;
    bool _window_closed_event = false;
};