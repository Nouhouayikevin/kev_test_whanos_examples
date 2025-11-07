/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** SfmlSprite.hpp
*/

// =================================================================
//  FICHIER: sfml/SfmlSprite.hpp
//  RÔLE:    Définit l'implémentation SFML de ISprite.
// =================================================================

#pragma once

#include "../interfaces/ISprite.hpp"
#include <SFML/Graphics/Sprite.hpp>

class SfmlSprite : public ISprite {
public:
    SfmlSprite();

    void setTexture(ITexture& texture) override;
    void setTextureRect(const IntRect& rect) override;
    void setPosition(const Vector2f& position) override;
    void setRotation(float angle) override;
    void setScale(const Vector2f& scale) override;
    void* getRawSprite() override;

private:
    sf::Sprite _sprite;
};