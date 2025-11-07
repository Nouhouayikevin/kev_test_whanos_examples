/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** ResourceManager.hpp
*/


#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <any>
#include "../Component.hpp"

class ISprite;
class IWindow;
class IFont;

class IWindow {
public:
    virtual bool isOpen() = 0;
    virtual void clear() = 0;
    virtual void draw(ISprite& sprite) = 0;
    virtual void drawRectangle(float x, float y, float width, float height, const Component::Core::Color& color) = 0;
    virtual void drawCircle(float x, float y, float radius, const Component::Core::Color& color) = 0;
    virtual void drawText(const std::string& text, float x, float y, IFont& font, int character_size, const Component::Core::Color& color) = 0;
    virtual void display() = 0;
    virtual void close() = 0;
    virtual IWindow & GetRawWindow() = 0;
};