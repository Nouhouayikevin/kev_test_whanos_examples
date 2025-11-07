/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** IFont.hpp
*/

// =================================================================
//  FICHIER: interfaces/IFont.hpp
//  RÔLE:    Définit le contrat pour une ressource de police.
// =================================================================

#pragma once

#include <string>

class IFont {
public:
    virtual ~IFont() = default;

    virtual bool loadFromFile(const std::string& path) = 0;
    virtual void* getRawFont() = 0;
};
