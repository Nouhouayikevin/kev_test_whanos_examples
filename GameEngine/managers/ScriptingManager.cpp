/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** ScriptingManager.cpp
*/

#include "../../Include/GameEngine_Include/managers/ScriptingManager.hpp"
#include "../../Include/GameEngine_Include/core/GameEngine.hpp"
#include "../../Include/GameEngine_Include/core/LuaEntity.hpp"
#include "../../Include/All/Component.hpp"

ScriptingManager::ScriptingManager(GameEngine& engine) : _engine(engine)
{
    _lua.open_libraries(sol::lib::base,
                        sol::lib::math,
                        sol::lib::string,
                        sol::lib::table,
                        sol::lib::io);

    _lua.new_usertype<LuaEntity>("LuaEntity",
        "setPosition", &LuaEntity::setPosition,
        "getPosition", &LuaEntity::getPosition,
        "setVelocity", &LuaEntity::setVelocity,
        "getVelocity", &LuaEntity::getVelocity,
        "get_velocity", &LuaEntity::get_velocity,
        "getHealth", &LuaEntity::getHealth,
        "get_health", &LuaEntity::get_health,
        "getId", &LuaEntity::getId,
        "setScale", &LuaEntity::setScale
    );

    sol::table engine_api = _lua.create_named_table("engine");

    engine_api["spawn_from_archetype"] = [this](const std::string& archetype, float x, float y) {
        // Utiliser le registre par défaut du ScriptingManager
        Entity new_entity = _engine.spawn_from_archetype(archetype, _default_register_group, Component::Core::Position{x, y});
        return static_cast<size_t>(new_entity);
    };
    
    engine_api["get_all_player_positions"] = [this]() -> sol::object {
        auto& registry = _engine.getRegistry(_default_register_group);
        auto& players = registry.get_components<Component::Gameplay::PlayerTag>();
        auto& positions = registry.get_components<Component::Core::Position>();
        
        sol::state_view lua(_lua);
        sol::table all_positions = lua.create_table();

        for (size_t i = 0; i < players.size(); ++i) {
            if (players[i] && positions[i]) {
                sol::table pos_table = lua.create_table();
                pos_table["x"] = positions[i]->x;
                pos_table["y"] = positions[i]->y;
                all_positions.add(pos_table);
            }
        }

        return all_positions;
    };

    engine_api["get_player_position"] = [this]() -> sol::object {

        auto result = _lua["engine"]["get_all_player_positions"]();

        if (result.valid()) {

            sol::object all_pos = result;

            if (all_pos.is<sol::table>() && all_pos.as<sol::table>().size() > 0) {
                return all_pos.as<sol::table>()[1];
            }
        }
        return sol::nil;
    };

    engine_api["set_velocity"] = [this](size_t entity_id, float dx, float dy) {
        auto& registry = _engine.getRegistry(_default_register_group);
        auto& velocities = registry.get_components<Component::Core::Velocity>();
        if (entity_id < velocities.size() && velocities[entity_id]) {
            velocities[entity_id]->dx = dx;
            velocities[entity_id]->dy = dy;
        }
    };
}

sol::state& ScriptingManager::getLuaState()
{
    return _lua;
}