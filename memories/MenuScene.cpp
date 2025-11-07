/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** MenuScene.cpp
*/

#include "../../Include/All/Scenes/MenuScene.hpp"

#include <iostream>

// --- Implémentation de la Scène Menu ---

void MenuScene::init(GameEngine& engine) {
    std::cout << "MenuScene: Initializing..." << std::endl;

    // --- 1. CHARGEMENT DES RESSOURCES ---
    // On charge la police principale qui sera utilisée pour tous les textes du menu.
    // On suppose que son ID est 0 si c'est la première police chargée.
    size_t main_font_id = engine.getResourceManager().load_font("Assets/fonts/main_font.ttf"); // Adapte le chemin

    // --- 2. ENREGISTREMENT DES COMPOSANTS ---
    // On déclare au Registry tous les types de composants que cette scène va utiliser.
    _registry.register_component<Component::Core::Position>();
    _registry.register_component<Component::Core::Hitbox>();
    _registry.register_component<Component::Graphics::RectangleShape>();
    _registry.register_component<Component::Graphics::UIText>();
    _registry.register_component<Component::UI::Clickable>();
    _registry.register_component<Component::Graphics::ButtonStateComponent>();
    _registry.register_component<Component::UI::Action>();
    _registry.register_component<Component::Graphics::StatefulAppearance>();

    // --- 3. CRÉATION DES ENTITÉS UI ---

    // -- Titre du jeu --
    Entity title = _registry.spawn_entity();
    title.set_name("GameTitle");
    title.add_component<Component::Core::Position>(Component::Core::Position(450, 100));
    title.add_component<Component::Graphics::UIText>(Component::Graphics::UIText("R-TYPE", main_font_id, 50, Component::Core::Color(255, 255, 255)));

    // -- Bouton "Jouer" --
    Entity play_button = _registry.spawn_entity();
    play_button.set_name("PlayButton");
    play_button.add_component<Component::Core::Position>(Component::Core::Position(540, 300));
    play_button.add_component<Component::Core::Hitbox>(Component::Core::Hitbox(200, 50));
    play_button.add_component<Component::Graphics::RectangleShape>(Component::Graphics::RectangleShape());
    play_button.add_component<Component::Graphics::UIText>(Component::Graphics::UIText("Jouer", main_font_id, 24, Component::Core::Color(255, 255, 255)));
    play_button.add_component<Component::UI::Clickable>(Component::UI::Clickable());
    play_button.add_component<Component::Graphics::ButtonStateComponent>(Component::Graphics::ButtonStateComponent());
    play_button.add_component<Component::UI::Action>(Component::UI::Action([&engine]() {
        std::cout << "Play button clicked! (Scene switching logic would go here)" << std::endl;
        // NOTE: Ici tu appellerais ton SceneManager pour changer de scène.
        // Par exemple : engine.getSceneManager().switch_to_scene("game", engine);
    }));
    
    Component::Graphics::StatefulAppearance play_appearance;
    play_appearance.color_map[Component::Core::ButtonState::IDLE]   = {50, 50, 50, 200};
    play_appearance.color_map[Component::Core::ButtonState::HOVER]  = {100, 100, 100, 220};
    play_appearance.color_map[Component::Core::ButtonState::PRESSED]= {20, 20, 20, 255};
    play_button.add_component<Component::Graphics::StatefulAppearance>(Component::Graphics::StatefulAppearance());

    // -- Bouton "Quitter" --
    Entity quit_button = _registry.spawn_entity();
    quit_button.set_name("QuitButton");
    quit_button.add_component<Component::Core::Position>(Component::Core::Position(540, 400));
    quit_button.add_component<Component::Core::Hitbox>(Component::Core::Hitbox(200, 50));
    quit_button.add_component<Component::Graphics::RectangleShape>(Component::Graphics::RectangleShape());
    quit_button.add_component<Component::Graphics::UIText>(Component::Graphics::UIText("Quitter", main_font_id, 24, Component::Core::Color(255, 255, 255)));
    quit_button.add_component<Component::UI::Clickable>(Component::UI::Clickable());
    quit_button.add_component<Component::Graphics::ButtonStateComponent>(Component::Graphics::ButtonStateComponent());
    quit_button.add_component<Component::UI::Action>(Component::UI::Action([&engine]() {
        std::cout << "Quit button clicked! Closing window..." << std::endl;
        engine.getWindowManager().close();
    }));
    quit_button.add_component<Component::Graphics::StatefulAppearance>(Component::Graphics::StatefulAppearance());

    // --- 4. ENREGISTREMENT DES SYSTÈMES UI ---
    
    // -- Système d'Interaction de la Souris --
    _registry.add_system([this](GameEngine& engine, float dt) {
        // La logique est DANS la lambda. On récupère les composants ici.
        auto& positions = _registry.get_components<Component::Core::Position>();
        auto& hitboxes = _registry.get_components<Component::Core::Hitbox>();
        auto& clickables = _registry.get_components<Component::UI::Clickable>();
        auto& states = _registry.get_components<Component::Graphics::ButtonStateComponent>();
        auto& actions = _registry.get_components<Component::UI::Action>();
        
        auto mouse_pos = engine.getInputManager().getMousePosition();

        for (size_t i = 0; i < clickables.size(); ++i) {
            if (!positions[i] || !hitboxes[i] || !clickables[i] || !states[i]) continue;
            
            auto& pos = *positions[i];
            auto& hitbox = *hitboxes[i];
            auto& state = *states[i];

            bool is_hovered = (mouse_pos.x >= pos.x && mouse_pos.x <= pos.x + hitbox.width &&
                               mouse_pos.y >= pos.y && mouse_pos.y <= pos.y + hitbox.height);

            if (is_hovered) {
                // NOTE: Tu auras besoin de 'isMouseButtonPressed' et 'isMouseButtonReleased' dans ton IInputManager
                if (engine.getInputManager().isMouseButtonPressed(MouseButton::Left)) {
                    state.state = Component::Core::ButtonState::PRESSED;
                } else if (engine.getInputManager().isMouseButtonPressed(MouseButton::Left) && state.state == Component::Core::ButtonState::PRESSED) {
                    if (actions[i] && actions[i]->on_click) actions[i]->on_click();
                    state.state = Component::Core::ButtonState::HOVER;
                } else if (state.state != Component::Core::ButtonState::PRESSED) {
                    state.state = Component::Core::ButtonState::HOVER;
                }
                
                 // Logique simplifiée en attendant les méthodes de l'input manager
                if(state.state != Component::Core::ButtonState::PRESSED) state.state = Component::Core::ButtonState::HOVER;

            } else {
                state.state = Component::Core::ButtonState::IDLE;
            }
        }
    });

    // -- Système de mise à jour de l'apparence --
    _registry.add_system([this](GameEngine& engine, float dt) {
        auto& states = _registry.get_components<Component::Graphics::ButtonStateComponent>();
        auto& appearances = _registry.get_components<Component::Graphics::StatefulAppearance>();
        auto& rects = _registry.get_components<Component::Graphics::RectangleShape>();

        for (size_t i = 0; i < states.size(); ++i) {
            if (!states[i] || !appearances[i] || !rects[i]) continue;
            
            auto& state = *states[i];
            auto& appearance = *appearances[i];
            auto& rect = *rects[i];

            if (appearance.color_map.count(state.state)) {
                rect.fill_color = appearance.color_map.at(state.state);
            }
        }
    });

    std::cout << "MenuScene: Initialized successfully." << std::endl;
}

void MenuScene::update(GameEngine& engine, float delta_time) {
    // La méthode update exécute simplement tous les systèmes enregistrés.
    _registry.run_systems(engine, delta_time);
}

void MenuScene::render(GameEngine& engine) {
    IWindow& window = engine.getWindowManager();
    ResourceManager& resources = engine.getResourceManager();
    
    // On récupère les composants nécessaires pour le rendu.
    auto& positions = _registry.get_components<Component::Core::Position>();
    auto& hitboxes = _registry.get_components<Component::Core::Hitbox>();
    auto& rects = _registry.get_components<Component::Graphics::RectangleShape>();
    auto& texts = _registry.get_components<Component::Graphics::UIText>();

    // Rendu des formes (les fonds de boutons)
    for (size_t i = 0; i < rects.size(); ++i) {
        if (rects[i] && positions[i] && hitboxes[i]) {
            // NOTE: Ton IWindow aura besoin d'une méthode comme 'drawRectangle'.
            window.drawRectangle(
                positions[i]->x, positions[i]->y,
                hitboxes[i]->width, hitboxes[i]->height,
                rects[i]->fill_color
            );
        }
    }

    // Rendu des textes
    for (size_t i = 0; i < texts.size(); ++i) {
        if (texts[i] && positions[i]) {
            // Pour centrer le texte sur les boutons, on aurait besoin de mesurer le texte.
            // Pour l'instant, on le dessine à la position de l'entité.
            // NOTE: Ton IWindow aura besoin d'une méthode comme 'drawText'.
            IFont& font = resources.get_font(texts[i]->font_id);
            
            // On pourrait centrer le texte ici si on pouvait mesurer le texte
            float text_x = positions[i]->x + 10;
            float text_y = positions[i]->y + 10;

            // C'EST MAINTENANT ACTIF !
            window.drawText(
                texts[i]->content,
                text_x, text_y,
                font, // On passe l'objet font, pas l'ID
                texts[i]->character_size,
                texts[i]->color
            );
        }
    }
}