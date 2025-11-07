/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** SfmlResourceFactory.cpp
*/

#include "../../Include/All/sfml/SfmlResourceFactory.hpp"

#include "../../Include/All/sfml/SfmlTexture.hpp"
#include "../../Include/All/sfml/SfmlSound.hpp"
#include "../../Include/All/sfml/SfmlFont.hpp"
#include "../../Include/All/sfml/SfmlMusic.hpp"
#include "../../Include/All/sfml/SfmlSprite.hpp"

std::unique_ptr<ITexture> SfmlResourceFactory::createTexture() {
    return std::make_unique<SfmlTexture>();
}

std::unique_ptr<ISound> SfmlResourceFactory::createSound() {
    return std::make_unique<SfmlSound>();
}

std::unique_ptr<IMusic> SfmlResourceFactory::createMusic() {
    return std::make_unique<SfmlMusic>();
}

std::unique_ptr<IFont> SfmlResourceFactory::createFont() {
    return std::make_unique<SfmlFont>();
}

std::unique_ptr<ISprite> SfmlResourceFactory::createSprite() {
    return std::make_unique<SfmlSprite>();
}
