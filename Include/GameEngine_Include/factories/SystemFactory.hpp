/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** SystemFactory.hpp - Factory pour créer dynamiquement des systèmes depuis leur nom
*/

#pragma once

#include <string>
#include <memory>
#include <functional>
#include <unordered_map>
#include <iostream>
#include <stdexcept>
#include "../../All/interfaces/ISystem.hpp"

namespace Engine {

/**
 * @brief Factory pour créer des systèmes dynamiquement depuis leur nom
 * 
 * Cette classe permet de transformer un nom de système (string) en instance concrète.
 * C'est le pont entre le JSON (qui contient des noms) et le code C++ (qui a les classes).
 */
class SystemFactory {
public:
    using SystemCreator = std::function<std::unique_ptr<ISystem>(const std::string&)>;

    /**
     * @brief Enregistre un système dans la factory
     * 
     * @tparam T Le type du système (doit hériter de ISystem)
     * @param name Le nom du système (utilisé dans le JSON)
     * 
     * Exemple : register_system<PlayerControlSystem>("PlayerControlSystem");
     */
    template<typename T>
    static void register_system(const std::string& name) {
        get_creators()[name] = [](const std::string& registry_group) -> std::unique_ptr<ISystem> {
            return std::make_unique<T>(registry_group);
        };
        std::cout << "[SystemFactory] Registered system: " << name << std::endl;
    }

    /**
     * @brief Crée une instance d'un système depuis son nom
     * 
     * @param name Le nom du système (doit avoir été enregistré avant)
     * @param registry_group Le groupe de registry à passer au constructeur
     * @return std::unique_ptr<ISystem> L'instance créée
     * 
     * @throws std::runtime_error Si le système n'existe pas dans la factory
     */
    static std::unique_ptr<ISystem> create(const std::string& name, const std::string& registry_group) {
        auto& creators = get_creators();
        
        if (creators.count(name) == 0) {
            std::cerr << "[SystemFactory] ERROR: System '" << name << "' not registered in factory!" << std::endl;
            std::cerr << "[SystemFactory] Available systems: ";
            for (const auto& [sys_name, _] : creators) {
                std::cerr << sys_name << ", ";
            }
            std::cerr << std::endl;
            
            throw std::runtime_error("System '" + name + "' not found in SystemFactory!");
        }
        
        return creators[name](registry_group);
    }

    /**
     * @brief Vérifie si un système est enregistré
     */
    static bool is_registered(const std::string& name) {
        return get_creators().count(name) > 0;
    }

    /**
     * @brief Récupère la liste de tous les systèmes disponibles
     */
    static std::vector<std::string> get_available_systems() {
        std::vector<std::string> names;
        for (const auto& [name, _] : get_creators()) {
            names.push_back(name);
        }
        return names;
    }

    /**
     * @brief Affiche tous les systèmes enregistrés (debug)
     */
    static void print_registered_systems() {
        std::cout << "[SystemFactory] Registered systems (" << get_creators().size() << "):" << std::endl;
        for (const auto& [name, _] : get_creators()) {
            std::cout << "  - " << name << std::endl;
        }
    }

private:
    /**
     * @brief Singleton pour stocker les créateurs de systèmes
     * 
     * On utilise un singleton pour éviter les problèmes d'ordre d'initialisation
     * des variables statiques entre différentes unités de compilation.
     */
    static std::unordered_map<std::string, SystemCreator>& get_creators() {
        static std::unordered_map<std::string, SystemCreator> creators;
        return creators;
    }
};

} // namespace Engine
