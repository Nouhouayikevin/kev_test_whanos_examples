/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** SfmlFont.hpp
*/

// =================================================================
//  FICHIER: sfml/SfmlFont.hpp
//  RÔLE:    Définit l'implémentation SFML de IFont.
// =================================================================

#pragma once

#include "../interfaces/IFont.hpp"
#include <SFML/Graphics/Font.hpp>

class SfmlFont : public IFont {
public:
    SfmlFont();
    
    bool loadFromFile(const std::string& path) override;
    void* getRawFont() override;

private:
    sf::Font _font;
};