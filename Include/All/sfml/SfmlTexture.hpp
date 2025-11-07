/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** SfmlTexture.hpp
*/


#pragma once

#include "../interfaces/ITexture.hpp"
#include <SFML/Graphics/Texture.hpp>

class SfmlTexture : public ITexture {
public:
    SfmlTexture();

    // Implémentation du contrat
    bool loadFromFile(const std::string& path) override;
    Vector2u getSize() const override;
    void* getRawTexture() override;

private:
    sf::Texture _texture; // La VRAIE texture SFML
};
