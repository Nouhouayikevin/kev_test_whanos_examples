/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** ISound.hpp
*/

// =================================================================
//  FICHIER: interfaces/ISound.hpp
//  RÔLE:    Définit le contrat pour un effet sonore court.
// =================================================================

#pragma once

#include <string>

class ISound {
public:
    virtual ~ISound() = default;

    virtual bool loadFromFile(const std::string& path) = 0;
    virtual void play() = 0;
    virtual void setVolume(float volume) = 0;
};