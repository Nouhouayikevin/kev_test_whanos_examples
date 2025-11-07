/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** SfmlSound.hpp
*/

// =================================================================
//  FICHIER: sfml/SfmlSound.hpp
//  RÔLE:    Définit l'implémentation SFML de ISound.
// =================================================================

#pragma once

#include "../interfaces/ISound.hpp"
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

class SfmlSound : public ISound {
public:
    SfmlSound();

    // Implémentation du contrat
    bool loadFromFile(const std::string& path) override;
    void play() override;
    void setVolume(float volume) override;

private:
    // En SFML, on a besoin de deux objets :
    // 1. Le SoundBuffer, qui contient les données audio (lourd).
    sf::SoundBuffer _buffer;
    // 2. Le Sound, qui est une "voix" qui peut jouer le buffer (léger).
    sf::Sound _sound;
};