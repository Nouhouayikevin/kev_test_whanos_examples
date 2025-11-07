/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** ISystem.hpp
*/

#pragma once

// On a besoin de la déclaration anticipée du moteur, car tous les systèmes y ont accès.
class GameEngine; 

/**
 * @brief Interface de base pour tous les systèmes du moteur.
 * Définit un contrat simple : un système est une unité de logique qui peut être exécutée.
 */
class ISystem {
public:
    virtual ~ISystem() = default;

    /**
     * @brief Exécute la logique du système pour une frame.
     * @param engine Le moteur, pour accéder aux managers (Registry, Input, etc.).
     * @param dt Le delta time, pour une logique indépendante de la framerate.
     */
    virtual void run(GameEngine& engine, float dt) = 0;
};
