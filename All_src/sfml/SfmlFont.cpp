/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** SfmlFont.cpp
*/

// =================================================================
//  FICHIER: sfml/SfmlFont.cpp
//  RÔLE:    Implémente la logique de SfmlFont.
// =================================================================

#include "../../Include/All/sfml/SfmlFont.hpp"

SfmlFont::SfmlFont() = default;

bool SfmlFont::loadFromFile(const std::string& path) {
    return _font.loadFromFile(path);
}

void* SfmlFont::getRawFont() {
    return &_font;
}