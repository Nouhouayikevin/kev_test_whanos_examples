/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** OnlineModeScene.hpp
*/

#pragma once

#include "../../All/interfaces/IScene.hpp" 
#include "../../GameEngine_Include/core/GameEngine.hpp" 
#include "../../GameEngine_Include/Registry.hpp" 

class OnlineModeScene : public IScene {
public:
    void init(GameEngine& engine) override;
    void update(GameEngine& engine, float delta_time) override;
    void render(GameEngine& engine) override;

private:
    Registry _registry;
    size_t _font_id;
    size_t _bg_texture_id;

    // Constantes de couleurs pour le survol
    const Component::Core::Color BTN_COLOR_IDLE = {0, 100, 200, 200};
    const Component::Core::Color BTN_COLOR_HOVER = {50, 150, 255, 220};
    const Component::Core::Color BTN_COLOR_PRESSED = {0, 50, 150, 255};
};