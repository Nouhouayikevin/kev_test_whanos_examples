/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** SfmlInputManager.cpp
*/

// =================================================================
//  FICHIER: sfml/SfmlInputManager.cpp
//  RÔLE:    Implémente la logique de SfmlInputManager.
// =================================================================

#include "../../Include/All/sfml/SfmlInputManager.hpp"
#include "../../Include/All/sfml/SfmlWindow.hpp"
#include "../../Include/GameEngine_Include/core/Event.hpp"

KeyCode translateSfmlKey(sf::Keyboard::Key key) {
    static const std::map<sf::Keyboard::Key, KeyCode> keyMap = {
        {sf::Keyboard::A, KeyCode::A}, {sf::Keyboard::B, KeyCode::B},
        {sf::Keyboard::C, KeyCode::C}, {sf::Keyboard::D, KeyCode::D},
        {sf::Keyboard::E, KeyCode::E}, {sf::Keyboard::F, KeyCode::F},
        {sf::Keyboard::G, KeyCode::G}, {sf::Keyboard::H, KeyCode::H},
        {sf::Keyboard::I, KeyCode::I}, {sf::Keyboard::J, KeyCode::J},
        {sf::Keyboard::K, KeyCode::K}, {sf::Keyboard::L, KeyCode::L},
        {sf::Keyboard::M, KeyCode::M}, {sf::Keyboard::N, KeyCode::N},
        {sf::Keyboard::O, KeyCode::O}, {sf::Keyboard::P, KeyCode::P},
        {sf::Keyboard::Q, KeyCode::Q}, {sf::Keyboard::R, KeyCode::R},
        {sf::Keyboard::S, KeyCode::S}, {sf::Keyboard::T, KeyCode::T},
        {sf::Keyboard::U, KeyCode::U}, {sf::Keyboard::V, KeyCode::V},
        {sf::Keyboard::W, KeyCode::W}, {sf::Keyboard::X, KeyCode::X},
        {sf::Keyboard::Y, KeyCode::Y}, {sf::Keyboard::Z, KeyCode::Z},
        {sf::Keyboard::Left, KeyCode::Left}, {sf::Keyboard::Right, KeyCode::Right},
        {sf::Keyboard::Up, KeyCode::Up}, {sf::Keyboard::Down, KeyCode::Down},
        {sf::Keyboard::Space, KeyCode::Space}, {sf::Keyboard::Enter, KeyCode::Enter},
        {sf::Keyboard::Escape, KeyCode::Escape}
    };
    auto it = keyMap.find(key);
    return (it != keyMap.end()) ? it->second : KeyCode::Unknown;
}

SfmlInputManager::SfmlInputManager(SfmlWindow& window) : _window(window) {}

void SfmlInputManager::update() {
    _previous_key_states = _key_states;

    Event event;
    while (_window.pollEvent(event)) {
        
        if (event.type == EventType::WindowClosed) {
            _window_closed_event = true;
        }

        if (event.type == EventType::KeyPressed) {
            _key_states[event.key.code] = true;
        }

        if (event.type == EventType::KeyReleased) {
            _key_states[event.key.code] = false;
        }

        if (event.type == EventType::MouseButtonPressed) {
            MouseButton button_code = (event.mouseButton.button);
            if (button_code != MouseButton::Unknown) {
                _mouse_button_states[button_code] = true;
            }
        }
        
        if (event.type == EventType::MouseButtonReleased) {
            MouseButton button_code = (event.mouseButton.button);
            if (button_code != MouseButton::Unknown) {
                _mouse_button_states[button_code] = false;
            }
        }

        if (event.type == EventType::MouseMoved) {
            _mouse_position = {event.mouseMove.x, event.mouseMove.y};
        }
    }
}

bool SfmlInputManager::isKeyReleased(KeyCode key) const {
    bool current_state = isKeyDown(key);
    auto it = _previous_key_states.find(key);
    bool previous_state = (it != _previous_key_states.end()) ? it->second : false;
    
    return !current_state && previous_state;
}

bool SfmlInputManager::isMouseButtonReleased(MouseButton button) const {
    bool current_state = isMouseButtonDown(button);
    auto it = _previous_mouse_button_states.find(button);
    bool previous_state = (it != _previous_mouse_button_states.end()) ? it->second : false;

    return !current_state && previous_state;
}

bool SfmlInputManager::isKeyDown(KeyCode key) const {
    auto it = _key_states.find(key);
    return (it != _key_states.end()) ? it->second : false;
}

bool SfmlInputManager::isKeyPressed(KeyCode key) const {
    bool current_state = isKeyDown(key);
    
    auto it = _previous_key_states.find(key);
    bool previous_state = (it != _previous_key_states.end()) ? it->second : false;
    
    return current_state && !previous_state;
}


bool SfmlInputManager::isMouseButtonDown(MouseButton button) const {
    auto it = _mouse_button_states.find(button);
    return (it != _mouse_button_states.end()) ? it->second : false;
}

bool SfmlInputManager::isMouseButtonPressed(MouseButton button) const {
    bool current_state = isMouseButtonDown(button);
    
    auto it = _previous_mouse_button_states.find(button);
    bool previous_state = (it != _previous_mouse_button_states.end()) ? it->second : false;
    
    return current_state && !previous_state;
}


Vector2i SfmlInputManager::getMousePosition() const {
    return _mouse_position;
}

bool SfmlInputManager::isWindowClosed() const {
    return _window_closed_event || isKeyPressed(KeyCode::Escape) ;
}