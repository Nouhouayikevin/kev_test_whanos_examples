/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** GameScene.hpp
*/

#pragma once

#include "../interfaces/IScene.hpp"
#include "../Component.hpp"
#include "../../../Server/include/InputQueue.hpp"
#include "../../GameEngine_Include/core/GameEngine.hpp"
#include "../../Server/include/Snapshot.hpp"
#include <string>

class OnlineGameScene_server : public IScene {
private:
    InputQueue& _inputQueue;
    float _enemy_spawn_timer = 2.0f;

public: 
    OnlineGameScene_server(InputQueue& inputQueue) : _inputQueue(inputQueue) {};
    void init(GameEngine& context) override;
    void update(GameEngine& context, float delta_time) override;
    void render(GameEngine& context) override;
    Snapshot init_entity_player(GameEngine& gameEngine, uint32_t player_id, uint32_t tick);

};
