/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** ScriptSystem.hpp
*/

#pragma once
#include "../interfaces/ISystem.hpp"
#include "../../GameEngine_Include/core/GameEngine.hpp"
#include "../../GameEngine_Include/core/LuaEntity.hpp"
#include <string>
#include "../../GameEngine_Include/factories/SystemAutoRegister.hpp"
class ScriptSystem : public ISystem {
public:
    ScriptSystem(std::string registerGroup = "default") {
        _registerGroup = registerGroup;
    }
    void run(GameEngine& gameEng, float dt) override {
        auto& registry = gameEng.getRegistry(_registerGroup);
        auto& lua_state = gameEng.getScriptingManager().getLuaState();
        auto& scripts = registry.get_components<Component::Gameplay::Script>();

        for (size_t i = 0; i < scripts.size(); ++i) {
            if (!scripts[i]) {
                continue;
            }
            
            auto& script = *scripts[i];

            try {
                if (!script.is_initialized) {
                    try {
                        // Créer l'entité Lua pour ce script AVEC le bon registre (UNE SEULE FOIS)
                        script.self_table["entity"] = LuaEntity(gameEng, i, _registerGroup);
                        
                        lua_state.script_file("Assets/scripts/" + script.script_path, script.env);

                        sol::function init_func = script.env["init"];
                        if (init_func.valid()) {
                            sol::protected_function_result result = init_func(script.self_table);
                            if (!result.valid()) {
                                sol::error err = result;
                                std::cerr << "[ScriptSystem] LUA ERROR in init (" << script.script_path << "): " << err.what() << std::endl;
                            }
                        }
                        script.is_initialized = true;
                    } catch (const std::exception& e) {
                        std::cerr << "[ScriptSystem] ERROR loading script (" << script.script_path << "): " << e.what() << std::endl;
                    }
                }

                // L'entité Lua est déjà créée lors de l'initialisation, pas besoin de la recréer à chaque frame !

                sol::function update_func = script.env["on_update"];
                if (update_func.valid()) {
                    try {
                        sol::protected_function_result result = update_func(script.self_table, dt);
                        if (!result.valid()) {
                            sol::error err = result;
                            std::cerr << "[ScriptSystem] LUA ERROR in on_update (" << script.script_path << "): " << err.what() << std::endl;
                        }
                    } catch (const sol::error& e) {
                        std::cerr << "[ScriptSystem] SOL ERROR in on_update (" << script.script_path << "): " << e.what() << std::endl;
                    } catch (const std::exception& e) {
                        std::cerr << "[ScriptSystem] STD ERROR in on_update (" << script.script_path << "): " << e.what() << std::endl;
                    }
                }
            } catch (const sol::error& e) {
                std::cerr << "[ScriptSystem] LUA ERROR in " << script.script_path << ": " << e.what() << std::endl;
                scripts[i].reset();
            }
        }
    };
private:
    std::string _registerGroup;

    
    // ✨ AUTO-REGISTRATION MAGIQUE ✨
    AUTO_REGISTER_SYSTEM(ScriptSystem, "ScriptSystem")
};
