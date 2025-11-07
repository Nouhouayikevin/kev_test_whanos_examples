/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** SfmlSprite.cpp
*/

// =================================================================
//  FICHIER: sfml/SfmlSprite.cpp
//  RÔLE:    Implémente la logique de SfmlSprite.
// =================================================================

#include "../../Include/All/sfml/SfmlSprite.hpp"
#include "../../Include/All/interfaces/ITexture.hpp"
#include <SFML/Graphics/Texture.hpp>

SfmlSprite::SfmlSprite() = default;

void SfmlSprite::setTexture(ITexture& texture) {
    void* raw_texture_ptr = texture.getRawTexture();
    sf::Texture* sfml_texture = static_cast<sf::Texture*>(raw_texture_ptr);
    _sprite.setTexture(*sfml_texture);
}

void SfmlSprite::setTextureRect(const IntRect& rect) {
    _sprite.setTextureRect(sf::IntRect(rect.x, rect.y, rect.w, rect.h));
}

void SfmlSprite::setPosition(const Vector2f& position) {
    _sprite.setPosition(position.x, position.y);
}

void SfmlSprite::setRotation(float angle) {
    _sprite.setRotation(angle);
}

void SfmlSprite::setScale(const Vector2f& scale) {
    _sprite.setScale(scale.x, scale.y);
}


void* SfmlSprite::getRawSprite() {
    return &_sprite;
}
