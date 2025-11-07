/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** IMusic.hpp
*/

// =================================================================
//  FICHIER: interfaces/IMusic.hpp
//  RÔLE:    Définit le contrat pour une piste musicale longue.
// =================================================================

#pragma once

#include <string>

class IMusic {
public:
    virtual ~IMusic() = default;

    // 'open' et non 'load' pour refléter le streaming
    virtual bool openFromFile(const std::string& path) = 0;
    
    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void setLoop(bool loop) = 0;
    virtual void setVolume(float volume) = 0;
    virtual float getVolume() const = 0;
    virtual bool isPlaying() const = 0;
};
