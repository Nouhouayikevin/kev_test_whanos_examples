/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** ScriptingManager.hpp
*/

#ifndef SCRIPTINGMANAGER_HPP_
#define SCRIPTINGMANAGER_HPP_

#include <string>
#include <sol/sol.hpp>

class GameEngine;

class ScriptingManager {
    public:
        ScriptingManager(GameEngine& engine);
        sol::state& getLuaState();
        
        // Définir le registre utilisé par défaut pour les appels Lua
        void setDefaultRegisterGroup(const std::string& group) { _default_register_group = group; }
        const std::string& getDefaultRegisterGroup() const { return _default_register_group; }

    private:
        GameEngine& _engine;
        sol::state _lua;
        std::string _default_register_group = "default";
};

#endif // SCRIPTINGMANAGER_HPP_