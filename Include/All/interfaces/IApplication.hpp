/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** IApplication.hpp
*/

#pragma once


class IApplication {
public:
    virtual ~IApplication() = default;

    /**
     * @brief Initialise l'application, configure le moteur et charge les ressources initiales.
     */
    virtual void initialize() = 0;

    /**
     * @brief Lance la boucle de jeu principale.
     * Cette méthode bloquera jusqu'à ce que l'application se termine.
     */
    virtual void run() = 0;

    /**
     * @brief Nettoie les ressources et arrête l'application proprement.
     */
    virtual void shutdown() = 0;
};