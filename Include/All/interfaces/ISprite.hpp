/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** ISprite.hpp
*/

// =================================================================
//  FICHIER: interfaces/ISprite.hpp
//  RÔLE:    Définit le contrat pour un objet graphique affichable
//           basé sur une texture.
// =================================================================

#pragma once

#include "ITexture.hpp"

struct Vector2f { float x; float y; };
struct IntRect { int x; int y; int w; int h; };

class ISprite {
public:
    virtual ~ISprite() = default;

    virtual void setTexture(ITexture& texture) = 0;
    virtual void setTextureRect(const IntRect& rect) = 0;
    virtual void setPosition(const Vector2f& position) = 0;
    virtual void setRotation(float angle) = 0;
    virtual void setScale(const Vector2f& scale) = 0;

    virtual void* getRawSprite() = 0;
};