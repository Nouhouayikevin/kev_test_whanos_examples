/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** SfmlWindow.hpp
*/


// =================================================================
//  FICHIER: sfml/SfmlWindow.hpp
//  RÔLE:    Définit l'implémentation SFML de l'interface IWindow.
// =================================================================

#pragma once

#include "../interfaces/IWindow.hpp"
#include "../interfaces/ISprite.hpp"
#include "../interfaces/IFont.hpp"
#include "../Component.hpp"
#include <SFML/Graphics.hpp> 

#include "../../GameEngine_Include/core/Event.hpp"

class SfmlWindow : public IWindow {
public:
    
    SfmlWindow(unsigned int width, unsigned int height, const std::string& title);
    ~SfmlWindow()  = default;

    
    bool isOpen() override;
    void clear() override;
    void draw(ISprite& sprite) override;
    void drawRectangle(float x, float y, float width, float height, const Component::Core::Color& color) override;
    void drawCircle(float x, float y, float radius, const Component::Core::Color& color);
    void drawText(const std::string& text, float x, float y, IFont& font, int character_size, const Component::Core::Color& color) override;
    void display() override;
    void close() override;
    IWindow& GetRawWindow();
    bool pollEvent(Event& out_event);
private:
    
    sf::RenderWindow _window;

    
    void translateEvent(const sf::Event& sfml_event, Event& our_event);
    KeyCode translateKey(sf::Keyboard::Key key);
};