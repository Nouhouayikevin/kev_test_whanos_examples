/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** LuaEntity.cpp
*/

#include "../../Include/GameEngine_Include/core/LuaEntity.hpp"
#include "../../Include/GameEngine_Include/core/GameEngine.hpp"
#include "../../Include/All/Component.hpp"

LuaEntity::LuaEntity(GameEngine& engine, size_t id, std::string registerGroup) 
    : _game_engine(engine), _entity_id(id), _register_group(registerGroup) {}

sol::table LuaEntity::getPosition() const {
    auto& registry = _game_engine.getRegistry(_register_group);
    auto& positions = registry.get_components<Component::Core::Position>();
    
    sol::state_view lua(_game_engine.getScriptingManager().getLuaState());
    sol::table position_table = lua.create_table();
    
    if (_entity_id < positions.size() && positions[_entity_id]) {
        position_table["x"] = positions[_entity_id]->x;
        position_table["y"] = positions[_entity_id]->y;
    } else {
        // Retourne 0, 0 par défaut si le composant n'existe pas
        position_table["x"] = 0.0f;
        position_table["y"] = 0.0f;
    }
    return position_table;
}

void LuaEntity::setPosition(float x, float y) {
    auto& registry = _game_engine.getRegistry(_register_group);
    if (_entity_id < registry.get_components<Component::Core::Position>().size()) {
         registry.get_components<Component::Core::Position>()[_entity_id].value().x = x;
         registry.get_components<Component::Core::Position>()[_entity_id].value().y = y;
    }
}

sol::table LuaEntity::getVelocity() const {
    auto& registry = _game_engine.getRegistry(_register_group);
    auto& velocities = registry.get_components<Component::Core::Velocity>();

    sol::state_view lua(_game_engine.getScriptingManager().getLuaState());
    sol::table velocity_table = lua.create_table();
    
    if (_entity_id < velocities.size() && velocities[_entity_id]) {
        velocity_table["dx"] = velocities[_entity_id]->dx;
        velocity_table["dy"] = velocities[_entity_id]->dy;
    } else {
        // Retourne 0, 0 par défaut si le composant n'existe pas
        velocity_table["dx"] = 0.0f;
        velocity_table["dy"] = 0.0f;
    }
    return velocity_table;
}

void LuaEntity::setVelocity(float dx, float dy) {
    auto& registry = _game_engine.getRegistry(_register_group);
     if (_entity_id < registry.get_components<Component::Core::Velocity>().size()) {
        registry.get_components<Component::Core::Velocity>()[_entity_id].value().dx = dx;
        registry.get_components<Component::Core::Velocity>()[_entity_id].value().dy = dy;
    }
}

void LuaEntity::setScale(float width, float height) {
    auto& registry = _game_engine.getRegistry(_register_group);
    auto& scales = registry.get_components<Component::Core::Scale>();

    if (_entity_id < scales.size() && scales[_entity_id]) {
        scales[_entity_id]->width = width;
        scales[_entity_id]->height = height;
    } else {
        registry.add_component(Entity(_entity_id, &registry), Component::Core::Scale{width, height});
    }
}

int LuaEntity::getHealth() const {
    auto& registry = _game_engine.getRegistry(_register_group);
    auto& healths = registry.get_components<Component::Gameplay::Health>();
    
    if (_entity_id < healths.size() && healths[_entity_id]) {
        return healths[_entity_id]->hp;
    }
    return 0;
}

std::size_t LuaEntity::getId() const {
    return _entity_id;
}