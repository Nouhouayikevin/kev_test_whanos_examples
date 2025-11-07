/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** IInputManager.hpp
*/

// =================================================================
//  FICHIER: interfaces/IInputManager.hpp
//  RÔLE:    Définit le contrat pour un service de gestion des
//           entrées utilisateur (clavier, souris).
// =================================-================================

#pragma once

#include "../../GameEngine_Include/core/Event.hpp"


struct Vector2i { int x; int y; };


class IInputManager {
public:
    virtual ~IInputManager() = default;

    virtual void update() = 0;

    virtual bool isKeyDown(KeyCode key) const = 0;
    virtual Vector2i getMousePosition() const = 0;

    virtual bool isMouseButtonDown(MouseButton button) const = 0;
    virtual bool isMouseButtonPressed(MouseButton button) const = 0; 
    virtual bool isMouseButtonReleased(MouseButton button) const = 0;
    virtual bool isKeyPressed(KeyCode key) const = 0;
    virtual bool isKeyReleased(KeyCode key) const = 0;
    virtual bool isWindowClosed() const = 0;
};