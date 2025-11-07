/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** IResourceFactory.hpp
*/

#pragma once

#include <memory>

class ITexture;
class ISound;
class IMusic;
class IFont;
class ISprite;

/**
 * @brief Interface pour une usine de création de ressources.
 * Chaque bibliothèque (SFML, Raylib) devra fournir une implémentation concrète
 * de cette interface.
 */
class IResourceFactory {
public:
    virtual ~IResourceFactory() = default;

    virtual std::unique_ptr<ITexture> createTexture() = 0;
    virtual std::unique_ptr<ISound> createSound() = 0;
    virtual std::unique_ptr<IMusic> createMusic() = 0;
    virtual std::unique_ptr<IFont> createFont() = 0;
    virtual std::unique_ptr<ISprite> createSprite() = 0;
};
