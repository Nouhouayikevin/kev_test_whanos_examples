/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** LevelManager.cpp
*/

#include "../../Include/GameEngine_Include/managers/LevelManager.hpp"
#include "../../Include/GameEngine_Include/core/GameEngine.hpp"
#include <fstream>
#include <iostream>

LevelManager::LevelManager(GameEngine& engine, std::string register_group) 
    : _game_engine(engine), _timer(0.0f), _next_event_index(0) {
        _register_group = register_group;
    }

void LevelManager::load(const std::string& level_path) {
    std::cout << "LevelManager: Loading level '" << level_path << "'..." << std::endl;
    _current_level_path = level_path;

    std::ifstream f(level_path);
    if (!f.is_open()) {
        throw std::runtime_error("LevelManager: Failed to open level file: " + level_path);
    }

    try {
        _level_data = json::parse(f);
    } catch (json::parse_error& e) {
        throw std::runtime_error("LevelManager: Failed to parse level JSON: " + std::string(e.what()));
    }
    
    clear_level(_register_group);
    _timer = 0.0f;
    _next_event_index = 0;
}

void LevelManager::update(float dt, std::string registry_group) {
    if (_level_data.is_null() || !_level_data.contains("events")) {
        return;
    }

    _timer += dt;

    const auto& events = _level_data["events"];

    // On utilise un 'while' pour déclencher plusieurs événements qui pourraient se produire dans la même frame
    while (_next_event_index < events.size() && _timer >= events[_next_event_index]["time"]) {
        const auto& event = events[_next_event_index];

        std::string action = event.value("action", "");

        if (action == "SPAWN") {
            std::string archetype = event.value("archetype", "");
            if (archetype.empty()) continue;

            // On crée un std::optional<Position>
            std::optional<Component::Core::Position> spawn_pos;
            if (event.contains("pos")) {
                auto pos_arr = event["pos"];
                spawn_pos = Component::Core::Position(pos_arr[0], pos_arr[1]);
            }

            std::cout << "LevelManager: Spawning '" << archetype << "'..." << std::endl;
            _game_engine.spawn_from_archetype(archetype, registry_group, spawn_pos);
        }
        
        _next_event_index++;
    }
}

void LevelManager::clear_level(std::string registry_group, bool kill_player) {
    std::cout << "LevelManager: Clearing level entities." << std::endl;
    auto& registry = _game_engine.getRegistry(registry_group);
    auto& positions = registry.get_components<Component::Core::Position>();
    auto& player_tag = registry.get_components<Component::Gameplay::PlayerTag>();
    for (size_t i = 0; i < positions.size(); ++i) {
        if (positions[i]) {
            if (player_tag[i] && !kill_player)
                continue;

            // On ne tue pas le joueur pour l'instant, mais on pourrait lse faire
            // en vérifiant si l'entité n'a pas un PlayerTag.
            registry.kill_entity(Entity(i, &registry));
        }
    }
}

void LevelManager::restart_current() {
    if (!_current_level_path.empty()) {
        std::cout << "LevelManager: Restarting current level." << std::endl;
        load(_current_level_path);
    }
}