/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** SfmlMusic.hpp
*/

// =================================================================
//  FICHIER: sfml/SfmlMusic.hpp
//  RÔLE:    Définit l'implémentation SFML de IMusic.
// =================================================================

#pragma once

#include "../interfaces/IMusic.hpp"
#include <SFML/Audio/Music.hpp>

class SfmlMusic : public IMusic {
public:
    SfmlMusic();

    // Implémentation du contrat
    bool openFromFile(const std::string& path) override;
    void play() override;
    void pause() override;
    void stop() override;
    void setLoop(bool loop) override;
    void setVolume(float volume) override;
    float getVolume() const override;
    bool isPlaying() const override;

private:
    sf::Music _music;
};