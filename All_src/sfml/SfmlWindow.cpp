/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** SfmlWindow.cpp
*/

// =================================================================
//  FICHIER: sfml/SfmlWindow.cpp
//  RÔLE:    Implémente la logique de la classe SfmlWindow.
// =================================================================
#include "../../Include/All/sfml/SfmlWindow.hpp"
#include "../../Include/All/interfaces/ISprite.hpp"

SfmlWindow::SfmlWindow(unsigned int width, unsigned int height, const std::string& title)
    : _window(sf::VideoMode(width, height), title) {
    _window.setFramerateLimit(120);
}

// --- Implémentation du contrat IWindow ---

bool SfmlWindow::isOpen() {
    return _window.isOpen();
}

void SfmlWindow::clear() {
    _window.clear(sf::Color::Black);
}

void SfmlWindow::draw(ISprite& sprite) {
    void* raw_sprite_ptr = sprite.getRawSprite();
    if (raw_sprite_ptr) {
        sf::Sprite* sfml_sprite = static_cast<sf::Sprite*>(raw_sprite_ptr);
        _window.draw(*sfml_sprite);
    }
}

void SfmlWindow::drawRectangle(float x, float y, float width, float height, const Component::Core::Color& color)
{
    sf::RectangleShape shape;
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(sf::Vector2f(x, y));
    
    shape.setFillColor(sf::Color(color.r, color.g, color.b, color.a));

    _window.draw(shape);
}

void SfmlWindow::drawCircle(float x, float y, float radius, const Component::Core::Color& color)
{
    sf::CircleShape shape(radius);

    shape.setOrigin(radius, radius);

    shape.setPosition(sf::Vector2f(x, y));
    
    shape.setFillColor(sf::Color(color.r, color.g, color.b, color.a));

    _window.draw(shape);
}

void SfmlWindow::drawText(const std::string& text, float x, float y, IFont& font, int character_size, const Component::Core::Color& color)
{
    void* raw_font_ptr = font.getRawFont();
    if (!raw_font_ptr) return;

    sf::Font* sfml_font = static_cast<sf::Font*>(raw_font_ptr);

    sf::Text text_obj;
    text_obj.setFont(*sfml_font);
    text_obj.setString(text);
    text_obj.setCharacterSize(character_size);
    text_obj.setPosition(sf::Vector2f(x, y));
    text_obj.setFillColor(sf::Color(color.r, color.g, color.b, color.a));

    _window.draw(text_obj);
}

void SfmlWindow::display() {
    _window.display();
}

void SfmlWindow::close() {
    _window.close();
}

IWindow& SfmlWindow::GetRawWindow() {
    return *this;
}



bool SfmlWindow::pollEvent(Event& out_event) {
    sf::Event sfml_event;
    if (_window.pollEvent(sfml_event)) {
        translateEvent(sfml_event, out_event);
        return true;
    }
    return false;
}


// --- Fonctions d'aide privées pour la traduction ---

KeyCode SfmlWindow::translateKey(sf::Keyboard::Key key) {
    static const std::map<sf::Keyboard::Key, KeyCode> keyMap = {
        {sf::Keyboard::A, KeyCode::A}, {sf::Keyboard::B, KeyCode::B},
        {sf::Keyboard::C, KeyCode::C}, {sf::Keyboard::D, KeyCode::D},
        {sf::Keyboard::E, KeyCode::E}, {sf::Keyboard::F, KeyCode::F},
        {sf::Keyboard::G, KeyCode::G}, {sf::Keyboard::H, KeyCode::H},
        {sf::Keyboard::I, KeyCode::I}, {sf::Keyboard::J, KeyCode::J},
        {sf::Keyboard::K, KeyCode::K}, {sf::Keyboard::L, KeyCode::L},
        {sf::Keyboard::M, KeyCode::M}, {sf::Keyboard::N, KeyCode::N},
        {sf::Keyboard::O, KeyCode::O}, {sf::Keyboard::P, KeyCode::P},
        {sf::Keyboard::Q, KeyCode::Q}, {sf::Keyboard::R, KeyCode::R},
        {sf::Keyboard::S, KeyCode::S}, {sf::Keyboard::T, KeyCode::T},
        {sf::Keyboard::U, KeyCode::U}, {sf::Keyboard::V, KeyCode::V},
        {sf::Keyboard::W, KeyCode::W}, {sf::Keyboard::X, KeyCode::X},
        {sf::Keyboard::Y, KeyCode::Y}, {sf::Keyboard::Z, KeyCode::Z},
        {sf::Keyboard::Left, KeyCode::Left}, {sf::Keyboard::Right, KeyCode::Right},
        {sf::Keyboard::Up, KeyCode::Up}, {sf::Keyboard::Down, KeyCode::Down},
        {sf::Keyboard::Space, KeyCode::Space}, {sf::Keyboard::Enter, KeyCode::Enter},
        {sf::Keyboard::Escape, KeyCode::Escape}
    };

    auto it = keyMap.find(key);
    return (it != keyMap.end()) ? it->second : KeyCode::Unknown;
}

void SfmlWindow::translateEvent(const sf::Event& sfml_event, Event& our_event) {
    switch (sfml_event.type) {
        case sf::Event::Closed:
            our_event.type = EventType::WindowClosed;
            break;
        case sf::Event::KeyPressed:
            our_event.type = EventType::KeyPressed;
            our_event.key.code = translateKey(sfml_event.key.code);
            break;
        case sf::Event::KeyReleased:
            our_event.type = EventType::KeyReleased;
            our_event.key.code = translateKey(sfml_event.key.code);
            break;
        case sf::Event::MouseMoved:
            our_event.type = EventType::MouseMoved;
            our_event.mouseMove.x = sfml_event.mouseMove.x;
            our_event.mouseMove.y = sfml_event.mouseMove.y;
            break;
        case sf::Event::MouseButtonPressed:
        {
            our_event.type = EventType::MouseButtonPressed;
            switch (sfml_event.mouseButton.button) {
                case sf::Mouse::Left:
                    our_event.mouseButton.button = MouseButton::Left;
                    break;
                case sf::Mouse::Right:
                    our_event.mouseButton.button = MouseButton::Right;
                    break;
                case sf::Mouse::Middle:
                    our_event.mouseButton.button = MouseButton::Middle;
                    break;
                default:
                    our_event.mouseButton.button = MouseButton::Unknown;
                    break;
            }

            our_event.mouseButton.x = sfml_event.mouseButton.x;
            our_event.mouseButton.y = sfml_event.mouseButton.y;
        }
            break;
        case sf::Event::MouseButtonReleased:
            our_event.type = EventType::MouseButtonReleased;
            switch (sfml_event.mouseButton.button) {
                case sf::Mouse::Left:
                    our_event.mouseButton.button = MouseButton::Left;
                    break;
                case sf::Mouse::Right:
                    our_event.mouseButton.button = MouseButton::Right;
                    break;
                case sf::Mouse::Middle:
                    our_event.mouseButton.button = MouseButton::Middle;
                    break;
                default:
                    our_event.mouseButton.button = MouseButton::Unknown;
                    break;
            }
            our_event.mouseButton.x = sfml_event.mouseButton.x;
            our_event.mouseButton.y = sfml_event.mouseButton.y;
            break;
        default:
            our_event.type = EventType::Unknown;
            break;
    }
}