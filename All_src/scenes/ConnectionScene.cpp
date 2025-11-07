// /* ------------------------------------------------------------------------------------ *
// *                                                                                      *
// * EPITECH PROJECT - Tue, Oct, 2025                                                     *
// * Title           - G-CPP-500-COT-5-1-rtype-24                                         *
// * Description     -                                                                    *
// *     ConnexionScene                                                                   *
// *                                                                                      *
// * ------------------------------------------------------------------------------------ *
// *                                                                                      *
// *         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░             *
// *         ▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒             *
// *         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓             *
// *         █  ███████  ██████████  ███████  ████  ███████  ████  █  ████  █             *
// *         █        █  ███████        ████  ████        ██      ██  ████  █             *
// *                                                                                      *
// * ------------------------------------------------------------------------------------ */

// #include "../../Include/All/Scenes/ConnectionScene.hpp"
// #include "../../Include/All/interfaces/ISprite.hpp"
// #include "../../Include/All/Scenes/OnlineClientGameScene.hpp"
// #include <algorithm>
// #include <iostream>

// const std::string BG_LAYER_1_PATH = "Assets/sprites/parallax.jpg";
// static size_t bg1_texture_id;

// struct TextInput {
//     std::string content;
//     bool is_active = false;
//     std::string placeholder;

//     TextInput(const std::string& initial_content, const std::string& placeholder_text)
//         : content(initial_content), placeholder(placeholder_text) {}
// };

// void ConnectionScene::init(GameEngine& engine)
// {
//     std::cout << "ConnectionScene: Initializing..." << std::endl;

//     _main_font_id = engine.getResourceManager().load_font("Assets/fonts/main_font.ttf");
//     _bg_texture_id = engine.getResourceManager().load_texture(BG_LAYER_1_PATH);

//     _registry.register_component<Component::Core::Position>();
//     _registry.register_component<Component::Core::Velocity>();
//     _registry.register_component<Component::UI::Clickable>();
//     _registry.register_component<Component::Graphics::ButtonStateComponent>();
//     _registry.register_component<Component::UI::Action>();
//     _registry.register_component<Component::Core::Hitbox>();
//     _registry.register_component<Component::Graphics::RectangleShape>();
//     _registry.register_component<Component::Graphics::UIText>();
//     _registry.register_component<Component::Gameplay::ParallaxLayer>();
//     _registry.register_component<Component::Graphics::Sprite>();
//     _registry.register_component<TextInput>();

//     const float WINDOW_WIDTH = 1280.0f;
//     const float WINDOW_HEIGHT = 720.0f;
//     const float CENTER_X = WINDOW_WIDTH / 2.0f;

//     float title_font_size = 60;
//     float label_font_size = 35;
//     float input_font_size = 30;
//     float button_width = 300;
//     float button_height = 60;
//     float input_width = 300;
//     float input_height = 40;
//     float spacing = 50;

//     float total_height = title_font_size + 3 * (label_font_size + input_height + 10) + button_height + 3 * spacing;
//     float start_y = (WINDOW_HEIGHT - total_height) / 2.0f;

//     // Title
//     Entity title = _registry.spawn_entity();
//     title.set_name("GameTitle");
//     title.add_component<Component::Core::Position>(Component::Core::Position(CENTER_X - 150, start_y - 80));
//     title.add_component<Component::Graphics::UIText>(
//         Component::Graphics::UIText("R-TYPE", _main_font_id, title_font_size, Component::Core::Color(255, 255, 255)));

//     float y = start_y + title_font_size + 20;

//     // Name Input
//     {
//         Entity label = _registry.spawn_entity();
//         label.set_name("NameLabel");
//         label.add_component<Component::Core::Position>(Component::Core::Position(CENTER_X - 150, y));
//         label.add_component<Component::Graphics::UIText>(
//             Component::Graphics::UIText("Choose your name", _main_font_id, label_font_size, Component::Core::Color(255, 255, 255)));

//         y += label_font_size + 10;

//         Entity input = _registry.spawn_entity();
//         input.set_name("NameInput");
//         input.add_component<Component::Core::Position>(Component::Core::Position(CENTER_X - (input_width / 2), y));
//         input.add_component<Component::Core::Hitbox>(Component::Core::Hitbox(input_width, input_height));
//         input.add_component<Component::Graphics::RectangleShape>(Component::Graphics::RectangleShape());
//         auto& rect = input.get_component<Component::Graphics::RectangleShape>();
//         rect.fill_color = Component::Core::Color(100, 100, 100, 200);
//         input.add_component<Component::Graphics::UIText>(
//             Component::Graphics::UIText("MinD", _main_font_id, input_font_size, Component::Core::Color(255, 255, 255)));
//         input.add_component<Component::UI::Clickable>(Component::UI::Clickable());
//         input.add_component<TextInput>(TextInput("MinD", "MinD"));
//         y += input_height + spacing;
//     }

//     // Server IP Input
//     {
//         Entity label = _registry.spawn_entity();
//         label.set_name("IPLabel");
//         label.add_component<Component::Core::Position>(Component::Core::Position(CENTER_X - 150, y));
//         label.add_component<Component::Graphics::UIText>(
//             Component::Graphics::UIText("Server IP", _main_font_id, label_font_size, Component::Core::Color(255, 255, 255)));

//         y += label_font_size + 10;

//         Entity input = _registry.spawn_entity();
//         input.set_name("IPInput");
//         input.add_component<Component::Core::Position>(Component::Core::Position(CENTER_X - (input_width / 2), y));
//         input.add_component<Component::Core::Hitbox>(Component::Core::Hitbox(input_width, input_height));
//         input.add_component<Component::Graphics::RectangleShape>(Component::Graphics::RectangleShape());
//         auto& rect = input.get_component<Component::Graphics::RectangleShape>();
//         rect.fill_color = Component::Core::Color(100, 100, 100, 200);
//         input.add_component<Component::Graphics::UIText>(
//             Component::Graphics::UIText("127.0.0.1", _main_font_id, input_font_size, Component::Core::Color(255, 255, 255)));
//         input.add_component<Component::UI::Clickable>(Component::UI::Clickable());
//         input.add_component<TextInput>(TextInput("127.0.0.1", "127.0.0.1"));
//         y += input_height + spacing;
//     }

//     // Server Port Input
//     {
//         Entity label = _registry.spawn_entity();
//         label.set_name("PortLabel");
//         label.add_component<Component::Core::Position>(Component::Core::Position(CENTER_X - 150, y));
//         label.add_component<Component::Graphics::UIText>(
//             Component::Graphics::UIText("Server PORT", _main_font_id, label_font_size, Component::Core::Color(255, 255, 255)));

//         y += label_font_size + 10;

//         Entity input = _registry.spawn_entity();
//         input.set_name("PortInput");
//         input.add_component<Component::Core::Position>(Component::Core::Position(CENTER_X - (input_width / 2), y));
//         input.add_component<Component::Core::Hitbox>(Component::Core::Hitbox(input_width, input_height));
//         input.add_component<Component::Graphics::RectangleShape>(Component::Graphics::RectangleShape());
//         auto& rect = input.get_component<Component::Graphics::RectangleShape>();
//         rect.fill_color = Component::Core::Color(100, 100, 100, 200);
//         input.add_component<Component::Graphics::UIText>(
//             Component::Graphics::UIText("4444", _main_font_id, input_font_size, Component::Core::Color(255, 255, 255)));
//         input.add_component<Component::UI::Clickable>(Component::UI::Clickable());
//         input.add_component<TextInput>(TextInput("4444", "4444"));
//         y += input_height + spacing;
//     }

//     // Connect Button
//     {
//         Entity btn = _registry.spawn_entity();
//         btn.set_name("ConnectButton");
//         btn.add_component<Component::Core::Position>(Component::Core::Position(CENTER_X - (button_width / 2), y));
//         btn.add_component<Component::Core::Hitbox>(Component::Core::Hitbox(button_width, button_height));
//         btn.add_component<Component::Graphics::RectangleShape>(Component::Graphics::RectangleShape());
//         auto& rect = btn.get_component<Component::Graphics::RectangleShape>();
//         rect.fill_color = Component::Core::Color(100, 100, 100, 200);
//         btn.add_component<Component::Graphics::UIText>(
//             Component::Graphics::UIText("Connect", _main_font_id, 35, Component::Core::Color(255, 255, 255)));
//         btn.add_component<Component::UI::Clickable>(Component::UI::Clickable());
//         btn.add_component<Component::Graphics::ButtonStateComponent>(Component::Graphics::ButtonStateComponent());
//         btn.add_component<Component::UI::Action>(Component::UI::Action([&engine, this]() {
//             std::cout << "Connecting to server..." << std::endl;
//             auto name = _registry.get_entity_by_name("NameInput")->get_component<TextInput>().content;
//             auto ip = _registry.get_entity_by_name("IPInput")->get_component<TextInput>().content;
//             auto port = _registry.get_entity_by_name("PortInput")->get_component<TextInput>().content;
//             std::cout << "Name: " << name << ", IP: " << ip << ", Port: " << port << std::endl;
//             engine.getSceneManager().switch_to_scene("online_game", engine);
//         }));
//     }

//     // Background
//     Entity bg1 = _registry.spawn_entity();
//     _registry.add_component(bg1, Component::Core::Position(0, 0));
//     _registry.add_component(bg1, Component::Core::Velocity(-50.0f, 0));
//     _registry.add_component(bg1, Component::Graphics::Sprite(_bg_texture_id, WINDOW_WIDTH, WINDOW_HEIGHT, 0));
//     _registry.add_component(bg1, Component::Gameplay::ParallaxLayer(-50.0f));

//     Entity bg2 = _registry.spawn_entity();
//     _registry.add_component(bg2, Component::Core::Position(WINDOW_WIDTH, 0));
//     _registry.add_component(bg2, Component::Core::Velocity(-50.0f, 0));
//     _registry.add_component(bg2, Component::Graphics::Sprite(_bg_texture_id, WINDOW_WIDTH, WINDOW_HEIGHT, 0));
//     _registry.add_component(bg2, Component::Gameplay::ParallaxLayer(-50.0f));

//     // Input handling system
//     _registry.add_system([this](GameEngine& engine, float dt) {
//         auto& positions = _registry.get_components<Component::Core::Position>();
//         auto& hitboxes = _registry.get_components<Component::Core::Hitbox>();
//         auto& inputs = _registry.get_components<TextInput>();
//         auto& texts = _registry.get_components<Component::Graphics::UIText>();
//         auto& states = _registry.get_components<Component::Graphics::ButtonStateComponent>();
//         auto& actions = _registry.get_components<Component::UI::Action>();

//         auto mouse_pos = engine.getInputManager().getMousePosition();
//         bool mouse_clicked = engine.getInputManager().isMouseButtonPressed(MouseButton::Left);

//         // Handle input field selection
//         for (size_t i = 0; i < inputs.size(); i++) {
//             if (!positions[i] || !hitboxes[i] || !inputs[i] || !texts[i]) continue;

//             auto& pos = *positions[i];
//             auto& hitbox = *hitboxes[i];
//             auto& input = *inputs[i];
//             auto& text = *texts[i];

//             bool is_hovered = (mouse_pos.x >= pos.x && mouse_pos.x <= pos.x + hitbox.width &&
//                                mouse_pos.y >= pos.y && mouse_pos.y <= pos.y + hitbox.height);

//             if (is_hovered && mouse_clicked) {
//                 for (size_t j = 0; j < inputs.size(); j++) {
//                     if (inputs[j]) inputs[j]->is_active = (j == i);
//                 }
//             }

//             if (input.is_active) {
//                 // Handle Backspace
//                 if (engine.getInputManager().isKeyPressed(KeyCode::Backspace) && !input.content.empty()) {
//                     input.content.pop_back();
//                 }
//                 // Handle letters for Name field
//                 if (i == 0) { // Name field
//                     for (KeyCode code = KeyCode::A; code <= KeyCode::Z; code = static_cast<KeyCode>(static_cast<int>(code) + 1)) {
//                         if (engine.getInputManager().isKeyPressed(code)) {
//                             input.content += static_cast<char>('A' + (static_cast<int>(code) - static_cast<int>(KeyCode::A)));
//                         }
//                     }
//                 }
//                 // Handle numbers and period for IP and Port fields
//                 if (i == 1 || i == 2) { // IP or Port field
//                     for (KeyCode code = KeyCode::N0; code <= KeyCode::N9; code = static_cast<KeyCode>(static_cast<int>(code) + 1)) {
//                         if (engine.getInputManager().isKeyPressed(code)) {
//                             input.content += static_cast<char>('0' + (static_cast<int>(code) - static_cast<int>(KeyCode::N0)));
//                         }
//                     }
//                 }
//                 // Handle period for IP field
//                 if (i == 1 && engine.getInputManager().isKeyPressed(KeyCode::Period)) {
//                     input.content += '.';
//                 }
//                 // Update displayed text
//                 text.content = input.content.empty() ? input.placeholder : input.content;
//             }
//         }

//         // Handle button interaction
//         for (size_t i = 0; i < states.size(); i++) {
//             if (!positions[i] || !hitboxes[i] || !states[i]) continue;

//             auto& pos = *positions[i];
//             auto& hitbox = *hitboxes[i];
//             auto& state_comp = *states[i];
//             auto current_state = state_comp.state;

//             bool is_hovered = (mouse_pos.x >= pos.x && mouse_pos.x <= pos.x + hitbox.width &&
//                                mouse_pos.y >= pos.y && mouse_pos.y <= pos.y + hitbox.height);

//             if (!is_hovered) {
//                 state_comp.state = Component::Core::ButtonState::IDLE;
//                 continue;
//             }

//             if (current_state == Component::Core::ButtonState::IDLE) {
//                 state_comp.state = Component::Core::ButtonState::HOVER;
//             }

//             if (mouse_clicked) {
//                 state_comp.state = Component::Core::ButtonState::PRESSED;
//             }

//             if (state_comp.state == Component::Core::ButtonState::PRESSED &&
//                 !engine.getInputManager().isMouseButtonDown(MouseButton::Left)) {
//                 if (actions[i] && actions[i]->on_click) {
//                     actions[i]->on_click();
//                 }
//                 state_comp.state = Component::Core::ButtonState::HOVER;
//             }
//         }
//     });

//     std::cout << "ConnectionScene initialized for window " << WINDOW_WIDTH << "x" << WINDOW_HEIGHT << std::endl;
// }

// void ConnectionScene::update(GameEngine& engine, float delta_time)
// {
//     auto& positions = _registry.get_components<Component::Core::Position>();
//     auto& velocities = _registry.get_components<Component::Core::Velocity>();

//     const float WINDOW_WIDTH = 1280.0f;

//     for (size_t i = 0; i < positions.size(); i++) {
//         if (positions[i] && velocities[i]) {
//             positions[i]->x += velocities[i]->dx * delta_time;
//             if (positions[i]->x <= -WINDOW_WIDTH)
//                 positions[i]->x = WINDOW_WIDTH - 1;
//         }
//     }

//     _registry.run_systems(engine, delta_time);
// }

// void ConnectionScene::render(GameEngine& engine)
// {
//     IWindow& window = engine.getWindowManager();
//     ResourceManager& resources = engine.getResourceManager();

//     auto& positions = _registry.get_components<Component::Core::Position>();
//     auto& hitboxes = _registry.get_components<Component::Core::Hitbox>();
//     auto& rects = _registry.get_components<Component::Graphics::RectangleShape>();
//     auto& texts = _registry.get_components<Component::Graphics::UIText>();
//     auto& sprites = _registry.get_components<Component::Graphics::Sprite>();
//     auto& inputs = _registry.get_components<TextInput>();

//     struct RenderableEntity {
//         int z_index;
//         Component::Core::Position position;
//         Component::Graphics::Sprite sprite;
//     };
//     std::vector<RenderableEntity> renderables;
//     for (size_t i = 0; i < sprites.size() && i < positions.size(); i++) {
//         if (sprites[i] && positions[i])
//             renderables.push_back({sprites[i]->z_index, *positions[i], *sprites[i]});
//     }
//     std::sort(renderables.begin(), renderables.end(), [](const auto& a, const auto& b) {
//         return a.z_index < b.z_index;
//     });

//     for (const auto& renderable : renderables) {
//         ITexture& texture = resources.get_texture(renderable.sprite.texture_id);
//         auto sprite = resources.createSprite();
//         sprite->setTexture(texture);
//         sprite->setPosition({renderable.position.x, renderable.position.y});
//         sprite->setTextureRect({
//             renderable.sprite.source_rect.x,
//             renderable.sprite.source_rect.y,
//             renderable.sprite.source_rect.width,
//             renderable.sprite.source_rect.height
//         });
//         window.draw(*sprite);
//     }

//     for (size_t i = 0; i < rects.size(); i++) {
//         if (rects[i] && positions[i] && hitboxes[i]) {
//             window.drawRectangle(
//                 positions[i]->x, positions[i]->y,
//                 hitboxes[i]->width, hitboxes[i]->height,
//                 rects[i]->fill_color
//             );
//         }
//     }

//     for (size_t i = 0; i < texts.size(); i++) {
//         if (texts[i] && positions[i]) {
//             IFont& font = resources.get_font(texts[i]->font_id);
//             float x_offset = 20;
//             float y_offset = 10;

//             if (texts[i]->content == "Connect" ||
//                 texts[i]->content == "MinD" || texts[i]->content == "127.0.0.1" || texts[i]->content == "4444" ||
//                 (inputs[i] && inputs[i]->content == texts[i]->content))
//                 x_offset += 30;

//             window.drawText(
//                 texts[i]->content,
//                 positions[i]->x + x_offset,
//                 positions[i]->y + y_offset,
//                 font,
//                 texts[i]->character_size,
//                 texts[i]->color
//             );
//         }
//     }
// }