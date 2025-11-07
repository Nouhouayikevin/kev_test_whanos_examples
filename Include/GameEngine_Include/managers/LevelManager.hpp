/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** LevelManager.hpp
*/

#pragma once

#include <string>
#include "../core/json.hpp"

class GameEngine;

using json = nlohmann::json;

class LevelManager {
public:
    LevelManager(GameEngine& engine, std::string _register_group = "default");

    void load(const std::string& level_path);
    
    void update(float dt, std::string registry_group = "default");

    void clear_level(std::string registry_group, bool kill_player = false);

    void restart_current();
    void set_register_group(std::string register_group) {
        _register_group = register_group;
    }

private:
    GameEngine& _game_engine;
    json _level_data;
    float _timer;
    size_t _next_event_index;
    std::string _current_level_path;
    std::string _register_group;
};