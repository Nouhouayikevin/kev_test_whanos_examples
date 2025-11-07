/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** IClock.hpp
*/

#pragma once
#include <chrono>

/**
 * @brief Une classe Clock de haute précision, indépendante de la plateforme.
 * Utilise std::chrono::steady_clock pour garantir une mesure du temps
 * qui ne peut pas reculer (contrairement à l'horloge système).
 */
class Clock {
public:
    Clock();

    /**
     * @brief Renvoie le temps écoulé depuis le dernier appel à restart() (en secondes).
     * Cet appel ne réinitialise pas l'horloge.
     * @return float Le temps écoulé en secondes.
     */
    float getElapsedTime() const;

    /**
     * @brief Réinitialise l'horloge et renvoie le temps écoulé depuis la dernière réinitialisation.
     * C'est la fonction la plus courante pour une boucle de jeu (delta time).
     * @return float Le temps écoulé en secondes.
     */
    float restart();

private:
    // Le point de départ temporel pour la mesure actuelle.
    std::chrono::steady_clock::time_point _start_point;
};