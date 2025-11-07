/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** SfmlMusic.cpp
*/

// =================================================================
//  FICHIER: sfml/SfmlMusic.cpp
//  RÔLE:    Implémente la logique de SfmlMusic.
// =================================================================

#include "../../Include/All/sfml/SfmlMusic.hpp"

SfmlMusic::SfmlMusic() = default;

bool SfmlMusic::openFromFile(const std::string& path) {
    return _music.openFromFile(path);
}

void SfmlMusic::play() {
    _music.play();
}

void SfmlMusic::stop() {
    _music.stop();
}

void SfmlMusic::setLoop(bool loop) {
    _music.setLoop(loop);
}

void SfmlMusic::setVolume(float volume) {
    _music.setVolume(volume);
}

void SfmlMusic::pause() {
    _music.pause();
}

float SfmlMusic::getVolume() const {
    return _music.getVolume();
}

bool SfmlMusic::isPlaying() const {
    return _music.getStatus() == sf::Music::Playing;
}
