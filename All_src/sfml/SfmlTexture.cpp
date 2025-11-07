/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** SfmlTexture.cpp
*/

#include "../../Include/All/sfml/SfmlTexture.hpp"

SfmlTexture::SfmlTexture() = default;

bool SfmlTexture::loadFromFile(const std::string& path) {
    return _texture.loadFromFile(path);
}

Vector2u SfmlTexture::getSize() const {
    sf::Vector2u sfmlSize = _texture.getSize();
    return {sfmlSize.x, sfmlSize.y};
}

void* SfmlTexture::getRawTexture() {
    return &_texture;
}