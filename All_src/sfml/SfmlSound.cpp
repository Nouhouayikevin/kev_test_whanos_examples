/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** SfmlSound.cpp
*/

// =================================================================
//  FICHIER: sfml/SfmlSound.cpp
//  RÔLE:    Implémente la logique de SfmlSound.
// =================================================================

#include "../../Include/All/sfml/SfmlSound.hpp"

SfmlSound::SfmlSound() = default;

bool SfmlSound::loadFromFile(const std::string& path) {
    if (!_buffer.loadFromFile(path)) {
        return false;
    }
    _sound.setBuffer(_buffer);
    return true;
}

void SfmlSound::play() {
    _sound.play();
}

void SfmlSound::setVolume(float volume) {
    _sound.setVolume(volume);
}