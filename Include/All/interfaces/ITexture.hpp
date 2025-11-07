/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** ITexture.hpp
*/

// =================================================================
//  FICHIER: interfaces/ITexture.hpp
//  RÔLE:    Définit le contrat pour une ressource de type texture.
// =================================================================

#pragma once

#include <string>

struct Vector2u {
    unsigned int x;
    unsigned int y;
};

class ITexture {
public:
    virtual ~ITexture() = default;

    virtual bool loadFromFile(const std::string& path) = 0;
    virtual Vector2u getSize() const = 0;

    virtual void* getRawTexture() = 0;
};