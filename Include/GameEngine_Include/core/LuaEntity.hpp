/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** LuaEntity.hpp
*/

#ifndef LUAENTITY_HPP
#define LUAENTITY_HPP

#include <sol/sol.hpp>

class GameEngine;

class LuaEntity {
    public:
    LuaEntity(GameEngine& engine, std::size_t id, std::string registerGroup = "default");

    void setPosition(float x, float y);
    sol::table getPosition() const;

    void setVelocity(float dx, float dy);
    sol::table getVelocity() const;
    
    // Alias pour compatibilité avec le script Lua
    sol::table get_velocity() const { return getVelocity(); }

    void setScale(float width, float height);

    int getHealth() const;
    
    // Alias pour compatibilité avec le script Lua
    int get_health() const { return getHealth(); }

    std::size_t getId() const;

    private:
    GameEngine& _game_engine;
    std::size_t _entity_id;
    std::string _register_group;
};

#endif // LUAENTITY_HPP