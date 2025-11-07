/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** SfmlResourceFactory.hpp
*/

#pragma once

#include "../interfaces/IResourceFactory.hpp"

class SfmlResourceFactory : public IResourceFactory {
public:
    std::unique_ptr<ITexture> createTexture() override;
    std::unique_ptr<ISound> createSound() override;
    std::unique_ptr<IMusic> createMusic() override;
    std::unique_ptr<IFont> createFont() override;
    std::unique_ptr<ISprite> createSprite() override;
};