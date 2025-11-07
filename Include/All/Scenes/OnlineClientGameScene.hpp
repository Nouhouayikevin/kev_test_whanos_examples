/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** GameScene.hpp
*/

#pragma once

#include "../interfaces/IScene.hpp"
#include "../Component.hpp"
#include "../../GameEngine_Include/core/GameEngine.hpp"
#include "../../Server/include/Snapshot.hpp"
#include <unordered_map>

class ClientOnlineGameScene : public IScene {
    private:

        enum class ConnectionState {
            WAITING_FOR_SERVER,
            PLAYING
        };
        ConnectionState _state;
        std::unordered_map<uint32_t, Entity> server_to_client_entity_map;
        void applySnapshot(Registry& registry, const Snapshot& snapshot);
        std::vector<Snapshot> _snapshots_to_process;

    public:
        void queueSnapshot(const Snapshot& snapshot);
        void init(GameEngine& context) override;
        void update(GameEngine& context, float delta_time) override;
        void render(GameEngine& context) override;
};
