/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** SceneConfig.hpp - Configuration data-driven pour les scènes
*/

#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "../../../Include/All/interfaces/nlohmann/json.hpp"

using json = nlohmann::json;

namespace Engine {

/**
 * @brief Configuration d'un groupe de systèmes
 */
struct SystemGroupConfig {
    std::string name;           // Nom du groupe (update, render, script, etc.)
    float frequency;            // Fréquence d'exécution (Hz)
    bool threaded;              // Exécution dans un thread séparé ?
    std::vector<std::string> systems; // Liste des noms de systèmes
    
    static SystemGroupConfig from_json(const json& j) {
        SystemGroupConfig config;
        config.name = j.value("name", "default");
        config.frequency = j.value("frequency", 60.0f);
        config.threaded = j.value("threaded", false);
        
        if (j.contains("systems") && j["systems"].is_array()) {
            for (const auto& sys : j["systems"]) {
                config.systems.push_back(sys.get<std::string>());
            }
        }
        
        return config;
    }
};

/**
 * @brief Action de lifecycle (event hook)
 */
struct LifecycleAction {
    std::string action;                      // Type d'action (log, load_music, etc.)
    std::map<std::string, std::string> params; // Paramètres de l'action
    
    static LifecycleAction from_json(const json& j) {
        LifecycleAction action_obj;
        action_obj.action = j.value("action", "");
        
        // Parser tous les paramètres (sauf "action" lui-même)
        for (auto& [key, value] : j.items()) {
            if (key != "action") {
                if (value.is_string()) {
                    action_obj.params[key] = value.get<std::string>();
                } else if (value.is_number()) {
                    action_obj.params[key] = std::to_string(value.get<double>());
                } else if (value.is_boolean()) {
                    action_obj.params[key] = value.get<bool>() ? "true" : "false";
                }
            }
        }
        
        return action_obj;
    }
};

/**
 * @brief Lifecycle hooks de la scène
 */
struct LifecycleHooks {
    std::vector<LifecycleAction> on_init;
    std::vector<LifecycleAction> on_update;
    std::vector<LifecycleAction> on_render;
    std::vector<LifecycleAction> on_exit;
    
    static LifecycleHooks from_json(const json& j) {
        LifecycleHooks hooks;
        
        if (j.contains("on_init") && j["on_init"].is_array()) {
            for (const auto& action_json : j["on_init"]) {
                hooks.on_init.push_back(LifecycleAction::from_json(action_json));
            }
        }
        
        if (j.contains("on_update") && j["on_update"].is_array()) {
            for (const auto& action_json : j["on_update"]) {
                hooks.on_update.push_back(LifecycleAction::from_json(action_json));
            }
        }
        
        if (j.contains("on_render") && j["on_render"].is_array()) {
            for (const auto& action_json : j["on_render"]) {
                hooks.on_render.push_back(LifecycleAction::from_json(action_json));
            }
        }
        
        if (j.contains("on_exit") && j["on_exit"].is_array()) {
            for (const auto& action_json : j["on_exit"]) {
                hooks.on_exit.push_back(LifecycleAction::from_json(action_json));
            }
        }
        
        return hooks;
    }
};

/**
 * @brief Configuration complète d'une scène
 */
struct SceneConfig {
    std::string scene_id;                       // Identifiant unique de la scène
    std::string registry_group;                 // Groupe de registry à utiliser
    std::string initial_level;                  // Niveau à charger au démarrage
    std::vector<SystemGroupConfig> system_groups; // Groupes de systèmes
    LifecycleHooks lifecycle;                   // Hooks de lifecycle
    
    /**
     * @brief Charge une configuration de scène depuis un objet JSON
     */
    static SceneConfig from_json(const json& j) {
        SceneConfig config;
        config.scene_id = j.value("scene_id", "unnamed_scene");
        config.registry_group = j.value("registry_group", "default");
        config.initial_level = j.value("initial_level", "");
        
        // Parser les groupes de systèmes
        if (j.contains("system_groups") && j["system_groups"].is_array()) {
            for (const auto& group_json : j["system_groups"]) {
                config.system_groups.push_back(SystemGroupConfig::from_json(group_json));
            }
        }
        
        // Parser le lifecycle
        if (j.contains("lifecycle")) {
            config.lifecycle = LifecycleHooks::from_json(j["lifecycle"]);
        }
        
        return config;
    }
    
    /**
     * @brief Charge toutes les scènes depuis un fichier JSON
     */
    static std::vector<SceneConfig> load_from_file(const std::string& filepath) {
        std::vector<SceneConfig> scenes;
        
        try {
            std::ifstream file(filepath);
            if (!file.is_open()) {
                throw std::runtime_error("Failed to open scenes file: " + filepath);
            }
            
            json j;
            file >> j;
            
            if (!j.contains("scenes") || !j["scenes"].is_array()) {
                throw std::runtime_error("Invalid scenes.json format: missing 'scenes' array");
            }
            
            for (const auto& scene_json : j["scenes"]) {
                scenes.push_back(SceneConfig::from_json(scene_json));
            }
            
            std::cout << "[SceneConfig] Loaded " << scenes.size() << " scene(s) from " << filepath << std::endl;
            
        } catch (const json::exception& e) {
            std::cerr << "[SceneConfig] JSON parsing error: " << e.what() << std::endl;
            throw std::runtime_error("Failed to parse scenes.json: " + std::string(e.what()));
        } catch (const std::exception& e) {
            std::cerr << "[SceneConfig] Error loading scenes: " << e.what() << std::endl;
            throw;
        }
        
        return scenes;
    }
    
    /**
     * @brief Valide la configuration (crash si invalide)
     */
    bool validate() const {
        // Vérifier que scene_id n'est pas vide
        if (scene_id.empty()) {
            std::cerr << "[SceneConfig] VALIDATION ERROR: scene_id is empty!" << std::endl;
            std::abort();
        }
        
        // Vérifier que registry_group n'est pas vide
        if (registry_group.empty()) {
            std::cerr << "[SceneConfig] VALIDATION ERROR: registry_group is empty for scene '" << scene_id << "'!" << std::endl;
            std::abort();
        }
        
        // Vérifier qu'il y a au moins un groupe de systèmes
        if (system_groups.empty()) {
            std::cerr << "[SceneConfig] VALIDATION ERROR: No system groups defined for scene '" << scene_id << "'!" << std::endl;
            std::abort();
        }
        
        // Vérifier que chaque groupe a au moins un système
        for (const auto& group : system_groups) {
            if (group.systems.empty()) {
                std::cerr << "[SceneConfig] VALIDATION ERROR: System group '" << group.name 
                          << "' has no systems in scene '" << scene_id << "'!" << std::endl;
                std::abort();
            }
        }
        
        std::cout << "[SceneConfig] ✅ Validation passed for scene '" << scene_id << "'" << std::endl;
        return true;
    }
};

} // namespace Engine
