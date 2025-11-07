/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** GameScene.cpp
*/

#include <iostream>
#include <random>
#include <algorithm>
#include "../../Include/GameEngine_Include/core/GameEngine.hpp"
#include "../../Include/All/Scenes/GameScene.hpp"
#include "../../Include/All/Component.hpp"
#include "../../Include/All/interfaces/ISprite.hpp"
#include "../../Include/All/interfaces/ISound.hpp"
#include "../../Include/All/sfml/SfmlSprite.hpp"
#include "../../Include/All/systems/ScriptSystem.hpp"
// #include "../../Include/All/systems/BossCollisionSystem.hpp"


const std::string PLAYER_TEXTURE_PATH = "Assets/sprites/r-typesheet42.gif";
const std::string ENEMY_TEXTURE_PATH = "Assets/sprites/r-typesheet5.gif";
const std::string MISSILE_TEXTURE_PATH = "Assets/sprites/r-typesheet30.gif";
const std::string BG_LAYER_1_PATH = "Assets/sprites/parallax.jpg";
const std::string BG_LAYER_2_PATH = "Assets/sprites/parallax.jpg";
const std::string LASER_SOUND_PATH = "Assets/sounds/r-type_sound_shoot.wav";

static size_t player_texture_id;
static size_t enemy_texture_id;
static size_t missile_texture_id;
static size_t bg1_texture_id;
static size_t bg2_texture_id;
static size_t laser_sound_id;

void GameScene::init(GameEngine& gameSceneEngine) {
    
    
    
    
    player_texture_id = gameSceneEngine.getResourceManager().load_texture(PLAYER_TEXTURE_PATH);
    enemy_texture_id = gameSceneEngine.getResourceManager().load_texture(ENEMY_TEXTURE_PATH);
    missile_texture_id = gameSceneEngine.getResourceManager().load_texture(MISSILE_TEXTURE_PATH);
    bg1_texture_id = gameSceneEngine.getResourceManager().load_texture(BG_LAYER_1_PATH);
    bg2_texture_id = gameSceneEngine.getResourceManager().load_texture(BG_LAYER_2_PATH);
    laser_sound_id = gameSceneEngine.getResourceManager().load_sound(LASER_SOUND_PATH);

    
    
    
    
    gameSceneEngine.getRegistry().register_component<Component::Core::Position>();
    gameSceneEngine.getRegistry().register_component<Component::Core::Velocity>();
    gameSceneEngine.getRegistry().register_component<Component::Core::Scale>();
    gameSceneEngine.getRegistry().register_component<Component::Core::Rotation>();
    gameSceneEngine.getRegistry().register_component<Component::Core::Hitbox>();

    gameSceneEngine.getRegistry().register_component<Component::Gameplay::PlayerTag>();
    gameSceneEngine.getRegistry().register_component<Component::Gameplay::Damage>();
    gameSceneEngine.getRegistry().register_component<Component::Gameplay::Controllable>();
    gameSceneEngine.getRegistry().register_component<Component::Gameplay::EnemyTag>();
    gameSceneEngine.getRegistry().register_component<Component::Gameplay::MissileTag>();
    gameSceneEngine.getRegistry().register_component<Component::Gameplay::ParallaxLayer>();
    gameSceneEngine.getRegistry().register_component<Component::Gameplay::AI>();
    gameSceneEngine.getRegistry().register_component<Component::Gameplay::Health>();
    gameSceneEngine.getRegistry().register_component<Component::Gameplay::Score>();
    gameSceneEngine.getRegistry().register_component<Component::Gameplay::Dead>();
    gameSceneEngine.getRegistry().register_component<Component::Gameplay::BossTag>();
    gameSceneEngine.getRegistry().register_component<Component::Gameplay::Script>();
    
    
    gameSceneEngine.getRegistry().register_component<Component::Graphics::Sprite>();
    gameSceneEngine.getRegistry().register_component<Component::Graphics::Animation>();
    gameSceneEngine.getRegistry().register_component<Component::UI::HealthBar>();
    
    gameSceneEngine.getRegistry().register_component<Component::Graphics::RectangleShape>();
    gameSceneEngine.getRegistry().register_component<Component::Graphics::CircleShape>();
    
    gameSceneEngine.getRegistry().register_component<Component::Audio::SoundEffect>();
    
    
    
    
    
    Entity player = gameSceneEngine.getRegistry().spawn_entity();
    player.set_name("PlayerOne");
    gameSceneEngine.getRegistry().add_component(player, Component::Core::Position(100.0f, 300.0f));
    gameSceneEngine.getRegistry().add_component(player, Component::Core::Velocity(0.0f, 0.0f));
    gameSceneEngine.getRegistry().add_component(player, Component::Graphics::Sprite(player_texture_id, 33, 21, 10));
    gameSceneEngine.getRegistry().add_component(player, Component::Graphics::Animation(0, 5, 1.0f, true));
    gameSceneEngine.getRegistry().add_component(player, Component::Gameplay::PlayerTag(0)); 
    gameSceneEngine.getRegistry().add_component(player, Component::Gameplay::Controllable{});
    gameSceneEngine.getRegistry().add_component(player, Component::Gameplay::Health(100, 100));
    gameSceneEngine.getRegistry().add_component(player, Component::Gameplay::Score(0));

    Entity health_bar = gameSceneEngine.getRegistry().spawn_entity();
    health_bar.set_name("PlayerOne_HealthBar");
    health_bar.add_component<Component::UI::HealthBar>(Component::UI::HealthBar(player, 0, -20.0f));
    health_bar.add_component<Component::Core::Position>(Component::Core::Position(0, 0)); 
    health_bar.add_component<Component::Core::Hitbox>(Component::Core::Hitbox(32, 5));  
    health_bar.add_component<Component::Graphics::RectangleShape>(Component::Core::Color(0,255,0,220));

    
    Entity bg1 = gameSceneEngine.getRegistry().spawn_entity();
    gameSceneEngine.getRegistry().add_component(bg1, Component::Core::Position(0, 0));
    gameSceneEngine.getRegistry().add_component(bg1, Component::Core::Velocity(-30.0f, 0));
    gameSceneEngine.getRegistry().add_component(bg1, Component::Graphics::Sprite(bg1_texture_id, 1920, 1080, 0));
    
    gameSceneEngine.getRegistry().add_component(bg1, Component::Gameplay::ParallaxLayer(-30.0f));

    Entity bg2 = gameSceneEngine.getRegistry().spawn_entity();
    gameSceneEngine.getRegistry().add_component(bg2, Component::Core::Position(0, 0));
    gameSceneEngine.getRegistry().add_component(bg2, Component::Core::Velocity(-60.0f, 0));
    gameSceneEngine.getRegistry().add_component(bg2, Component::Graphics::Sprite(bg2_texture_id, 1920, 1080, 1));
    
    gameSceneEngine.getRegistry().add_component(bg2, Component::Gameplay::ParallaxLayer(-60.0f));
    
    Entity bg1_bis = gameSceneEngine.getRegistry().spawn_entity();
    gameSceneEngine.getRegistry().add_component(bg1_bis, Component::Core::Position(1920, 0));
    gameSceneEngine.getRegistry().add_component(bg1_bis, Component::Core::Velocity(-30.0f, 0));
    gameSceneEngine.getRegistry().add_component(bg1_bis, Component::Graphics::Sprite(bg1_texture_id, 1920, 1080, 1));
    
    gameSceneEngine.getRegistry().add_component(bg1_bis, Component::Gameplay::ParallaxLayer(-30.0f));

    Entity bg2_bis = gameSceneEngine.getRegistry().spawn_entity();
    gameSceneEngine.getRegistry().add_component(bg2_bis, Component::Core::Position(1920, 0));
    gameSceneEngine.getRegistry().add_component(bg2_bis, Component::Core::Velocity(-60.0f, 0));
    gameSceneEngine.getRegistry().add_component(bg2_bis, Component::Graphics::Sprite(bg2_texture_id, 1920, 1080, 1));
    
    gameSceneEngine.getRegistry().add_component(bg2_bis, Component::Gameplay::ParallaxLayer(-60.0f));



    
    gameSceneEngine.getRegistry().add_system([](GameEngine& gameEng ,float dt) {
        auto& velocities = gameEng.getRegistry().get_components<Component::Core::Velocity>();
        auto const& controllables = gameEng.getRegistry().get_components<Component::Gameplay::Controllable>();

        for (size_t i = 0; i < velocities.size() && i < controllables.size(); i++) {
            auto const& dead_tags = gameEng.getRegistry().get_components<Component::Gameplay::Dead>();

            if (i < dead_tags.size() && dead_tags[i]) continue;
            if (velocities[i] && controllables[i]) {
                velocities[i]->dx = 0;
                velocities[i]->dy = 0;
                float player_speed = 200.0f;

                if (gameEng.getInputManager().isKeyDown(KeyCode::Up)) velocities[i]->dy = -player_speed;
                if (gameEng.getInputManager().isKeyDown(KeyCode::Down)) velocities[i]->dy = player_speed;
                if (gameEng.getInputManager().isKeyDown(KeyCode::Left)) velocities[i]->dx = -player_speed;
                if (gameEng.getInputManager().isKeyDown(KeyCode::Right)) velocities[i]->dx = player_speed;

                if (gameEng.getInputManager().isKeyPressed(KeyCode::Space)) {
                     auto const& positions = gameEng.getRegistry().get_components<Component::Core::Position>();
                    if (positions[i]) {
                        Entity missile = gameEng.getRegistry().spawn_entity();
                        gameEng.getRegistry().add_component(missile, Component::Core::Hitbox(10, 10));
                        gameEng.getRegistry().add_component(missile, Component::Gameplay::Damage(20));
                        gameEng.getRegistry().add_component(missile, Component::Core::Position(positions[i]->x + 50, positions[i]->y + 20));
                        gameEng.getRegistry().add_component(missile, Component::Core::Velocity(400.0f, 0.0f));
                        gameEng.getRegistry().add_component(missile, Component::Gameplay::MissileTag(0)); 
                        gameEng.getRegistry().add_component(missile, Component::Audio::SoundEffect(laser_sound_id, true));
                        missile.add_component<Component::Graphics::CircleShape>(Component::Graphics::CircleShape(5.0f, Component::Core::Color(255, 0, 0, 255)));
                    }
                }
            }
        }
    });

    
    gameSceneEngine.getRegistry().add_system([](GameEngine& gameEng ,float dt) {
        auto& positions = gameEng.getRegistry().get_components<Component::Core::Position>();
        auto const& velocities = gameEng.getRegistry().get_components<Component::Core::Velocity>();
        auto const& parallaxes = gameEng.getRegistry().get_components<Component::Gameplay::ParallaxLayer>();

        for (size_t i = 0; i < positions.size() && i < velocities.size(); i++) {
            auto const& dead_tags = gameEng.getRegistry().get_components<Component::Gameplay::Dead>();

            if (i < dead_tags.size() && dead_tags[i]) continue;
            if (positions[i] && velocities[i]) {
                if (positions[i]) { 

                    
                    
                    
                }
                positions[i]->x += velocities[i]->dx * (dt);
                positions[i]->y += velocities[i]->dy * (dt);
            }
        }
    });
    
    
    gameSceneEngine.getRegistry().add_system([](GameEngine& gameEng ,float dt) {
        auto& positions = gameEng.getRegistry().get_components<Component::Core::Position>();
        auto const& parallaxes = gameEng.getRegistry().get_components<Component::Gameplay::ParallaxLayer>();
        
        for (size_t i = 0; i < positions.size() && i < parallaxes.size(); i++) {
            auto const& dead_tags = gameEng.getRegistry().get_components<Component::Gameplay::Dead>();

            if (i < dead_tags.size() && dead_tags[i]) continue;
            if (positions[i] && parallaxes[i]) {
                if (positions[i]->x < -1920) {
                    positions[i]->x += 1920 * 2;
                }
            }
        }
    });

    
    gameSceneEngine.getRegistry().add_system([](GameEngine& gameEng ,float dt) {
        auto& sounds = gameEng.getRegistry().get_components<Component::Audio::SoundEffect>();
        for (auto& sound : sounds) {
            if (sound && sound->to_play) {
                gameEng.getResourceManager().get_sound(sound->sound_id).play();
                sound->to_play = false;
            }
        }
    });


    
    gameSceneEngine.getRegistry().add_system([](GameEngine& gameEng , float dt) {
        auto& animations = gameEng.getRegistry().get_components<Component::Graphics::Animation>();
        auto& sprites = gameEng.getRegistry().get_components<Component::Graphics::Sprite>();

        for (size_t i = 0; i < animations.size() && i < sprites.size(); i++) {
            auto const& dead_tags = gameEng.getRegistry().get_components<Component::Gameplay::Dead>();

            if (i < dead_tags.size() && dead_tags[i]) continue;
            if (animations[i] && sprites[i]) {
                auto& anim = *animations[i];
                auto& sprite = *sprites[i];

                anim.timer += (dt);

                if (anim.timer >= anim.frame_duration) {
                    anim.timer -= anim.frame_duration; 

                    
                    anim.current_frame++;

                    
                    if (anim.current_frame >= anim.num_frames) {
                        if (anim.loops) {
                            anim.current_frame = 0; 
                        } else {
                            anim.current_frame = anim.num_frames - 1; 
                            
                        }
                    }
                }

                int total_frame_index = anim.start_frame + anim.current_frame;
                sprite.source_rect.x = total_frame_index * sprite.frame_width;
                sprite.source_rect.y = 0;
                sprite.source_rect.width = sprite.frame_width;
                sprite.source_rect.height = sprite.frame_height;
            }
        }
    });

    gameSceneEngine.getRegistry().add_system([](GameEngine& gameEng, float dt) {
        auto& velocities = gameEng.getRegistry().get_components<Component::Core::Velocity>();
        auto& ais = gameEng.getRegistry().get_components<Component::Gameplay::AI>();

        for (size_t i = 0; i < ais.size(); i++) {
            auto const& dead_tags = gameEng.getRegistry().get_components<Component::Gameplay::Dead>();

            if (i < dead_tags.size() && dead_tags[i]) continue;
            if (!ais[i] || !velocities[i]) continue;

            auto& ai = *ais[i];
            auto& vel = *velocities[i];

            ai.time_elapsed += dt;
            float t = ai.time_elapsed;

            float base_dx = -100.0f;
            vel.dx = base_dx;

            switch (ai.type) {
                case Component::Gameplay::AIMovementType::VERTICAL_OSC: {
                    vel.dy = ai.amplitude * ai.frequency * cos(ai.frequency * t);
                    break;
                }
                case Component::Gameplay::AIMovementType::SINUSOIDAL: {
                    vel.dy = ai.amplitude * ai.frequency * cos(ai.frequency * t);
                    break;
                }
                case Component::Gameplay::AIMovementType::CIRCULAR: {
                    vel.dx = base_dx - ai.amplitude * ai.frequency * sin(ai.frequency * t);
                    vel.dy = ai.amplitude * ai.frequency * cos(ai.frequency * t);
                    break;
                }
                case Component::Gameplay::AIMovementType::LEMNISCATE: {

                    vel.dx = base_dx + ai.amplitude * ai.frequency * cos(ai.frequency * t);
                    vel.dy = (ai.amplitude / 2.0f) * (ai.frequency * 2.0f) * cos(ai.frequency * 2.0f * t);
                    break;
                }
                case Component::Gameplay::AIMovementType::STRAIGHT:
                default:
                    break;
            }
        }
    });

    gameSceneEngine.getRegistry().add_system([](GameEngine& gameEng, float dt) {
        auto& health_bars = gameEng.getRegistry().get_components<Component::UI::HealthBar>();
        auto& positions = gameEng.getRegistry().get_components<Component::Core::Position>();
        auto& rect_shapes = gameEng.getRegistry().get_components<Component::Graphics::RectangleShape>();
        auto& hitboxes = gameEng.getRegistry().get_components<Component::Core::Hitbox>();

        auto const& owner_healths = gameEng.getRegistry().get_components<Component::Gameplay::Health>();
        auto const& owner_positions = gameEng.getRegistry().get_components<Component::Core::Position>();

        for (size_t i = 0; i < health_bars.size(); i++) {
            auto const& dead_tags = gameEng.getRegistry().get_components<Component::Gameplay::Dead>();

            if (i < dead_tags.size() && dead_tags[i]) continue;
            if (!health_bars[i] || !positions[i] || !rect_shapes[i] || !hitboxes[i]) continue;

            auto& bar = *health_bars[i];
            auto& pos = *positions[i];
            auto& shape = *rect_shapes[i];
            auto& hitbox = *hitboxes[i];

            size_t owner_id = bar.owner;
            if (owner_id >= owner_healths.size() || owner_id >= owner_positions.size() || !owner_healths[owner_id] || !owner_positions[owner_id]) {
                continue;
            }
            auto const& owner_health = *owner_healths[owner_id];
            auto const& owner_pos = *owner_positions[owner_id];

            pos.x = owner_pos.x + bar.offset_x;
            pos.y = owner_pos.y + bar.offset_y;

            float health_percent = static_cast<float>(owner_health.hp) / static_cast<float>(owner_health.max_hp);
            health_percent = std::max(0.0f, std::min(1.0f, health_percent)); 

            const float max_bar_width = 32.0f; 
            hitbox.width = max_bar_width * health_percent;

            if (health_percent > 0.66f) {
                shape.fill_color = {0, 255, 0, 220}; 
            } else if (health_percent > 0.33f) {
                shape.fill_color = {255, 255, 0, 220}; 
            } else {
                shape.fill_color = {255, 0, 0, 220}; 
            }
        }
    });

    gameSceneEngine.getRegistry().add_system([](GameEngine& gameEng, float dt) {
        auto& registry = gameEng.getRegistry();

        
        auto& positions = registry.get_components<Component::Core::Position>();
        auto& hitboxes = registry.get_components<Component::Core::Hitbox>();
        auto& damages = registry.get_components<Component::Gameplay::Damage>();
        auto& missile_tags = registry.get_components<Component::Gameplay::MissileTag>();
        auto& enemy_tags = registry.get_components<Component::Gameplay::EnemyTag>();
        auto& healths = registry.get_components<Component::Gameplay::Health>();
        auto& scores = registry.get_components<Component::Gameplay::Score>();
        auto& player_tags = registry.get_components<Component::Gameplay::PlayerTag>();
        auto const& dead_tags = registry.get_components<Component::Gameplay::Dead>();

        
        
        for (size_t i = 0; i < missile_tags.size(); i++) {
            if (i < dead_tags.size() && dead_tags[i]) continue;
            
            if (  !missile_tags[i] || !positions[i] || !hitboxes[i] || !damages[i]) continue;


            auto& missile_pos = *positions[i];
            auto& missile_hitbox = *hitboxes[i];
            auto& missile_damage = *damages[i];
            auto& missile_tag = *missile_tags[i];

            for (size_t j = 0; j < enemy_tags.size(); j++) {
                if (j < dead_tags.size() && dead_tags[j]) continue;

                
                if (!enemy_tags[j] || !positions[j] || !hitboxes[j] || !healths[j]) continue;

                auto& enemy_pos = *positions[j];
                auto& enemy_hitbox = *hitboxes[j];
                auto& enemy_health = *healths[j];

                
                bool collision = 
                    missile_pos.x < enemy_pos.x + enemy_hitbox.width &&
                    missile_pos.x + missile_hitbox.width > enemy_pos.x &&
                    missile_pos.y < enemy_pos.y + enemy_hitbox.height &&
                    missile_pos.y + missile_hitbox.height > enemy_pos.y;

                if (collision) {
                    
                    enemy_health.hp -= missile_damage.amount;
                    std::cout << "Enemy " << j << " hit! HP: " << enemy_health.hp << std::endl;

                    
                    registry.add_component(Entity(i, &registry), Component::Gameplay::Dead{});

                    
                    if (enemy_health.hp <= 0) {
                        std::cout << "Enemy " << j << " destroyed!" << std::endl;
                        
                        
                        
                        if (scores[j]) {
                            
                            size_t owner_id = missile_tag.owner;
                            
                            
                            for (size_t p_idx = 0; p_idx < player_tags.size(); ++p_idx) {
                                if (player_tags[p_idx] && scores[p_idx]) {
                                    scores[p_idx]->value += scores[j]->value;
                                    std::cout << "Player score: " << scores[p_idx]->value << std::endl;
                                    break;
                                }
                            }
                        }

                        
                        registry.add_component(Entity(j, &registry), Component::Gameplay::Dead{});
                    }

                    
                    break; 
                }
            }
        }

    });

    gameSceneEngine.getRegistry().add_system([](GameEngine& gameEng, float dt) {
        auto& registry = gameEng.getRegistry();
        auto const& dead_tags = registry.get_components<Component::Gameplay::Dead>();
        
        std::vector<Entity> to_kill;
        for (size_t i = 0; i < dead_tags.size(); i++) {
            if (dead_tags[i]) {
                to_kill.emplace_back(Entity(i, &registry));
            }
        }

        for (const auto& e : to_kill) {
            registry.kill_entity(e);
        }
    });

    // Script System - exécute les scripts Lua attachés aux entités
    ScriptSystem scriptSystem;
    gameSceneEngine.getRegistry().add_system([scriptSystem](GameEngine& gameEng, float dt) mutable {
        scriptSystem.run(gameEng, dt);
    });

    // // Boss Collision System - gère les collisions spécifiques aux boss
    // BossCollisionSystem bossCollisionSystem;
    // gameSceneEngine.getRegistry().add_system([bossCollisionSystem](GameEngine& gameEng, float dt) mutable {
    //     bossCollisionSystem.run(gameEng, dt);
    // });
}

void GameScene::update(GameEngine& gameSceneEngine, float delta_time) {
    gameSceneEngine.getRegistry().run_systems(gameSceneEngine, delta_time);

    _enemy_spawn_timer -= (delta_time);
    if (_enemy_spawn_timer <= 0.0f) {
        Entity enemy = gameSceneEngine.getRegistry().spawn_entity();
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(50, 1000);
        float random_y = distrib(gen);

        std::vector<Component::Gameplay::AIMovementType> possible_ais = {
            Component::Gameplay::AIMovementType::SINUSOIDAL,
            Component::Gameplay::AIMovementType::VERTICAL_OSC,
            Component::Gameplay::AIMovementType::CIRCULAR,
            Component::Gameplay::AIMovementType::LEMNISCATE
        };
        std::uniform_int_distribution<> distrib_ai(0, possible_ais.size() - 1);
        Component::Gameplay::AIMovementType chosen_ai = possible_ais[distrib_ai(gen)];

        gameSceneEngine.getRegistry().add_component(enemy, Component::Core::Position(1000, random_y));
        gameSceneEngine.getRegistry().add_component(enemy, Component::Core::Velocity(-100.0f, 0.0f)); 
        gameSceneEngine.getRegistry().add_component(enemy, Component::Graphics::Sprite(enemy_texture_id,32, 33, 10));
        gameSceneEngine.getRegistry().add_component(enemy, Component::Graphics::Animation(0, 8, 1.0f, true));
        gameSceneEngine.getRegistry().add_component(enemy, Component::Gameplay::AI(chosen_ai, random_y));
        gameSceneEngine.getRegistry().add_component(enemy, Component::Gameplay::EnemyTag("BydosSlave"));
        gameSceneEngine.getRegistry().add_component(enemy, Component::Core::Hitbox(32, 33));
        
        gameSceneEngine.getRegistry().add_component(enemy, Component::Gameplay::Health(30, 30));

        Entity health_bar = gameSceneEngine.getRegistry().spawn_entity();
        
        health_bar.add_component<Component::UI::HealthBar>(Component::UI::HealthBar(enemy, 0, -15.0f));
        
        health_bar.add_component<Component::Core::Position>(Component::Core::Position(0, 0)); 
        health_bar.add_component<Component::Core::Hitbox>(Component::Core::Hitbox(25, 4));  
        health_bar.add_component<Component::Graphics::RectangleShape>(Component::Core::Color(0, 255, 0, 200));

        std::uniform_real_distribution<> time_distrib(1.0, 3.0);
        _enemy_spawn_timer = time_distrib(gen);
    }
}

void GameScene::render(GameEngine& gameEngine) {
    ResourceManager& resources = gameEngine.getResourceManager();
    IWindow& window = gameEngine.getWindowManager();
    Registry& registry = gameEngine.getRegistry();

    enum class RenderType { SPRITE, RECTANGLE, CIRCLE };

    struct Renderable {
        int z_index;
        RenderType type;

        const Component::Core::Position* pos;
        const Component::Core::Scale* scale = nullptr;
        const Component::Core::Rotation* rotation = nullptr;

        union {
            const Component::Graphics::Sprite* sprite_data;
            const Component::Graphics::RectangleShape* rect_data;
            const Component::Graphics::CircleShape* circle_data;
        };
        const Component::Core::Hitbox* hitbox = nullptr;
    };

    std::vector<Renderable> renderables;


    auto& positions = registry.get_components<Component::Core::Position>();
    auto& scales = registry.get_components<Component::Core::Scale>();
    auto& rotations = registry.get_components<Component::Core::Rotation>();
    auto& hitboxes = registry.get_components<Component::Core::Hitbox>();

    auto& sprites = registry.get_components<Component::Graphics::Sprite>();
    auto& rects = registry.get_components<Component::Graphics::RectangleShape>();
    auto& circles = registry.get_components<Component::Graphics::CircleShape>();
    
    size_t entity_count = positions.size();

    for (size_t i = 0; i < entity_count; i++) {
        if (!positions[i]) continue;

        Renderable item;
        item.pos = &(*positions[i]);
        item.scale = i < scales.size() && scales[i] ? &(*scales[i]) : nullptr;
        item.rotation = i < rotations.size() && rotations[i] ? &(*rotations[i]) : nullptr;
        item.hitbox = i < hitboxes.size() && hitboxes[i] ? &(*hitboxes[i]) : nullptr;

        if (sprites[i]) {
            item.type = RenderType::SPRITE;
            item.z_index = sprites[i]->z_index;
            item.sprite_data = &(*sprites[i]);
            renderables.push_back(item);
        } else if (rects[i] && item.hitbox) { 
            item.type = RenderType::RECTANGLE;
            item.z_index = 50;
            item.rect_data = &(*rects[i]);
            renderables.push_back(item);
        } else if (circles[i]) {
            item.type = RenderType::CIRCLE;
            item.z_index = 50;
            item.circle_data = &(*circles[i]);
            renderables.push_back(item);
        }
    }

    std::sort(renderables.begin(), renderables.end(), [](const auto& a, const auto& b) {
        return a.z_index < b.z_index;
    });

    for (const auto& renderable : renderables) {
        switch (renderable.type) {
            case RenderType::SPRITE: {
                if (!renderable.sprite_data) break;
                
                std::unique_ptr<ISprite> display_sprite = resources.createSprite();
                try {

                    ITexture& texture = resources.get_texture(renderable.sprite_data->texture_id);
                    display_sprite->setTexture(texture);
                } catch(const std::exception& e) {
                    continue;
                }

                display_sprite->setPosition({renderable.pos->x, renderable.pos->y});

                if (renderable.scale) {
                    display_sprite->setScale({renderable.scale->width, renderable.scale->height});
                }
                if (renderable.rotation) {
                    display_sprite->setRotation(renderable.rotation->angle);
                }

                display_sprite->setTextureRect({
                    renderable.sprite_data->source_rect.x,
                    renderable.sprite_data->source_rect.y,
                    renderable.sprite_data->source_rect.width,
                    renderable.sprite_data->source_rect.height
                });
                
                window.draw(*display_sprite);
                break;
            }
            case RenderType::RECTANGLE: {
                if (!renderable.rect_data || !renderable.hitbox) break;

                window.drawRectangle(
                    renderable.pos->x, renderable.pos->y,
                    renderable.hitbox->width, renderable.hitbox->height,
                    renderable.rect_data->fill_color
                );
                break;
            }
            case RenderType::CIRCLE: {
                if (!renderable.circle_data) break;

                window.drawCircle(
                    renderable.pos->x, renderable.pos->y,
                    renderable.circle_data->radius,
                    renderable.circle_data->fill_color
                );
                break;
            }
        }
    }
}
