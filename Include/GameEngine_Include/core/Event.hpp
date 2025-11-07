/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** Event.hpp
*/

// =================================================================
//  FICHIER: core/Event.hpp
//  RÔLE:    Définit une structure de données agnostique pour
//           représenter les événements discrets d'une fenêtre.
//           C'est un format de communication interne entre la couche
//           fenêtre et la couche input.
// =================================================================

#pragma once


enum class EventType {
    Unknown,
    WindowClosed,
    KeyPressed,
    KeyReleased,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved
};
enum class KeyCode {
    Unknown, Left, Right, Up, Down, Space, Escape, Enter, A, B, C, D, E, F, G, H,
    I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z
    // ...
};
enum class MouseButton {
    Unknown,
    Left,
    Right,
    Middle
};
struct Event {
    EventType type = EventType::Unknown;

    union {
        struct {
            KeyCode code;
        } key;

        struct {
            int x;
            int y;
        } mouseMove;

        struct {
            MouseButton button;
            int x;
            int y;
        } mouseButton;
    };
};