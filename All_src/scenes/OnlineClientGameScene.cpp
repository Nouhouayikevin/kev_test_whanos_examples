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
#include "../../Include/All/Scenes/OnlineClientGameScene.hpp"
#include "../../Include/All/Component.hpp"
#include "../../Include/All/interfaces/ISprite.hpp"
#include "../../Include/All/interfaces/ISound.hpp"
#include "../../Include/All/sfml/SfmlSprite.hpp"


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

void ClientOnlineGameScene::init(GameEngine& engine)
{
    _state = ConnectionState::WAITING_FOR_SERVER;
    player_texture_id = engine.getResourceManager().load_texture(PLAYER_TEXTURE_PATH);
    enemy_texture_id = engine.getResourceManager().load_texture(ENEMY_TEXTURE_PATH);
    missile_texture_id = engine.getResourceManager().load_texture(MISSILE_TEXTURE_PATH);
    bg1_texture_id = engine.getResourceManager().load_texture(BG_LAYER_1_PATH);
    bg2_texture_id = engine.getResourceManager().load_texture(BG_LAYER_2_PATH);
    laser_sound_id = engine.getResourceManager().load_sound(LASER_SOUND_PATH);

    // Les composants sont maintenant enregistrés automatiquement dans le constructeur du Registry

    
    Entity bg1 = engine.getRegistry().spawn_entity();
    engine.getRegistry().add_component(bg1, Component::Core::Position(0, 0));
    engine.getRegistry().add_component(bg1, Component::Core::Velocity(-30.0f, 0));
    engine.getRegistry().add_component(bg1, Component::Graphics::Sprite(bg1_texture_id, 1920, 1080, 0));
    engine.getRegistry().add_component(bg1, Component::Gameplay::ParallaxLayer(-30.0f));

    Entity bg2 = engine.getRegistry().spawn_entity();
    engine.getRegistry().add_component(bg2, Component::Core::Position(0, 0));
    engine.getRegistry().add_component(bg2, Component::Core::Velocity(-60.0f, 0));
    engine.getRegistry().add_component(bg2, Component::Graphics::Sprite(bg2_texture_id, 1920, 1080, 1));
    engine.getRegistry().add_component(bg2, Component::Gameplay::ParallaxLayer(-60.0f));
    
    Entity bg1_bis = engine.getRegistry().spawn_entity();
    engine.getRegistry().add_component(bg1_bis, Component::Core::Position(1920, 0));
    engine.getRegistry().add_component(bg1_bis, Component::Core::Velocity(-30.0f, 0));
    engine.getRegistry().add_component(bg1_bis, Component::Graphics::Sprite(bg1_texture_id, 1920, 1080, 1));
    engine.getRegistry().add_component(bg1_bis, Component::Gameplay::ParallaxLayer(-30.0f));

    Entity bg2_bis = engine.getRegistry().spawn_entity();
    engine.getRegistry().add_component(bg2_bis, Component::Core::Position(1920, 0));
    engine.getRegistry().add_component(bg2_bis, Component::Core::Velocity(-60.0f, 0));
    engine.getRegistry().add_component(bg2_bis, Component::Graphics::Sprite(bg2_texture_id, 1920, 1080, 1));
    engine.getRegistry().add_component(bg2_bis, Component::Gameplay::ParallaxLayer(-60.0f));

}


void ClientOnlineGameScene::queueSnapshot(const Snapshot& snapshot)
{
    _snapshots_to_process.push_back(snapshot);
}


void ClientOnlineGameScene::applySnapshot(Registry &registry, const Snapshot &snapshot)
{
    if (_state == ConnectionState::WAITING_FOR_SERVER && !snapshot.entities.empty()) {
        _state = ConnectionState::PLAYING;
        std::cout << "[ClientScene] Premier snapshot reçu ! Tick: " << snapshot.tick << ". Le jeu commence." << std::endl;
    }
    std::unordered_map<uint32_t, bool> seen_server_ids;
    for (const auto& entity_snapshot : snapshot.entities) {
        uint32_t server_id = entity_snapshot.id;
        seen_server_ids[server_id] = true;

        Entity client_entity(0, nullptr);
        
        if (server_to_client_entity_map.count(server_id)) {
            client_entity = server_to_client_entity_map.at(server_id);
        } else {
            client_entity = registry.spawn_entity();
            server_to_client_entity_map.insert({server_id, client_entity});
        }

        for (const auto& comp_data : entity_snapshot.components) {
            switch (comp_data.type) {
                case ComponentType::POSITION: {
                    // Dé-quantification: diviser par 10.0f
                    client_entity.add_component(Component::Core::Position{
                        static_cast<float>(comp_data.pos.x_q) / 10.0f,
                        static_cast<float>(comp_data.pos.y_q) / 10.0f
                    });
                    break;
                }
                case ComponentType::VELOCITY: {
                    // Dé-quantification: diviser par 10.0f
                    client_entity.add_component(Component::Core::Velocity{
                        static_cast<float>(comp_data.vel.x_q) / 10.0f,
                        static_cast<float>(comp_data.vel.y_q) / 10.0f
                    });
                    break;
                }
                case ComponentType::SCALE: {
                    // Dé-quantification: diviser par 10.0f
                     client_entity.add_component(Component::Core::Scale{
                        static_cast<float>(comp_data.scale.x_q) / 10.0f,
                        static_cast<float>(comp_data.scale.y_q) / 10.0f
                    });
                    break;
                }
                case ComponentType::ROTATION: {
                    // Dé-quantification: diviser par 10.0f
                    client_entity.add_component(Component::Core::Rotation{
                        static_cast<float>(comp_data.rotation.angle) / 10.0f
                    });
                    break;
                }
                case ComponentType::HITBOX: {
                    // Dé-quantification: diviser par 10.0f
                    client_entity.add_component(Component::Core::Hitbox{
                        static_cast<float>(comp_data.hitbox.w_q) / 10.0f,
                        static_cast<float>(comp_data.hitbox.h_q) / 10.0f
                    });
                    break;
                }
                case ComponentType::PLAYER_TAG: {
                    client_entity.add_component(Component::Gameplay::PlayerTag{
                        static_cast<size_t>(comp_data.player_tag.id_player)
                    });
                    break;
                }
                case ComponentType::DAMAGE: {
                    client_entity.add_component(Component::Gameplay::Damage{
                        static_cast<int>(comp_data.damage.damage)
                    });
                    break;
                }
                case ComponentType::CONTROLLABLE: {
                    client_entity.add_component(Component::Gameplay::Controllable{});
                    break;
                }
                case ComponentType::ENEMY_TAG: {
                    client_entity.add_component(Component::Gameplay::EnemyTag{
                        std::to_string(comp_data.enemy_tag.type)
                    });
                    break;
                }
                case ComponentType::MISSILE_TAG: {
                     client_entity.add_component(Component::Gameplay::MissileTag{
                        static_cast<size_t>(comp_data.missile_tag.owner)
                    });
                    break;
                }
                case ComponentType::PARALLAX_LAYER: {
                    client_entity.add_component(Component::Gameplay::ParallaxLayer{
                        static_cast<float>(comp_data.parallax_layer.scroll_speed)
                    });
                    break;
                }
                case ComponentType::HEALTH: {
                    client_entity.add_component(Component::Gameplay::Health{
                        static_cast<int>(comp_data.health.hp)
                    });
                    break;
                }
                case ComponentType::SCORE: {
                    client_entity.add_component(Component::Gameplay::Score{
                        static_cast<int>(comp_data.score.value)
                    });
                    break;
                }
                case ComponentType::DEAD: {
                    client_entity.add_component(Component::Gameplay::Dead{});
                    break;
                }
                case ComponentType::SPRITE: {
                    int server_tex_id = static_cast<int>(comp_data.sprite.texture_id);
                    int f_width = static_cast<int>(comp_data.sprite.frame_width);
                    int f_height = static_cast<int>(comp_data.sprite.frame_height);
                    int z = static_cast<int>(comp_data.sprite.z_index);

                    if (f_width <= 0 || f_height <= 0) {
                        std::cerr << "[ERREUR] Dimensions de sprite invalides reçues du serveur !" << std::endl;
                        break;
                    }

                    // Mapper les IDs du serveur vers les IDs locaux du client
                    size_t local_tex_id = 0;
                    switch (server_tex_id) {
                        case 0: local_tex_id = bg1_texture_id; break;      // parallax
                        case 1: local_tex_id = player_texture_id; break;   // player
                        case 2: local_tex_id = enemy_texture_id; break;    // enemy
                        case 3: local_tex_id = missile_texture_id; break;  // missile
                        default: 
                            std::cerr << "[WARN] Texture ID inconnu du serveur: " << server_tex_id << std::endl;
                            local_tex_id = player_texture_id; // Fallback
                            break;
                    }

                    client_entity.add_component(Component::Graphics::Sprite(local_tex_id, f_width, f_height, z));
                    break;
                }
                case ComponentType::ANIMATION: {
                    client_entity.add_component(Component::Graphics::Animation{
                        static_cast<int>(comp_data.animation.start_frame),
                        static_cast<int>(comp_data.animation.num_frames),
                        static_cast<float>(comp_data.animation.frame_duration) / 10.0f, // Dé-quantification
                        static_cast<bool>(comp_data.animation.loops)
                    });
                    break;
                }
                case ComponentType::HEALTH_BAR: {
                    client_entity.add_component(Component::UI::HealthBar{
                        static_cast<size_t>(comp_data.health_bar.owner),
                        static_cast<float>(comp_data.health_bar.offset_x) / 10.0f, // Dé-quantification
                        static_cast<float>(comp_data.health_bar.offset_y) / 10.0f  // Dé-quantification
                    });
                    break;
                }
                case ComponentType::RECTANGLE_SHAPE: {
                    // Les couleurs RGBA ne sont pas quantifiées
                    client_entity.add_component(Component::Graphics::RectangleShape{
                        Component::Core::Color(
                            static_cast<uint8_t>(comp_data.rect_shape.x_q),  // R
                            static_cast<uint8_t>(comp_data.rect_shape.y_q),  // G
                            static_cast<uint8_t>(comp_data.rect_shape.w_q),  // B
                            static_cast<uint8_t>(comp_data.rect_shape.h_q)   // A
                        )
                    });
                    break;
                }
                case ComponentType::CIRCLE_SHAPE: {
                    client_entity.add_component(Component::Graphics::CircleShape{
                        static_cast<float>(comp_data.circle_shape.radius) / 10.0f // Dé-quantification
                    });
                    break;
                }
                case ComponentType::SOUND_EFFECT: {
                    client_entity.add_component(Component::Audio::SoundEffect{
                        static_cast<size_t>(comp_data.sound_effect.sound_id),
                        static_cast<bool>(comp_data.sound_effect.to_play)
                    });
                    break;
                }
                case ComponentType::AI:
                case ComponentType::UNKNOWN:
                default: {
                    break;
                }
            }
        }
    }


    std::vector<uint32_t> ids_to_delete;
    for (const auto& pair : server_to_client_entity_map) {
        if (seen_server_ids.find(pair.first) == seen_server_ids.end()) {
            ids_to_delete.push_back(pair.first);
        }
    }

    for (uint32_t server_id : ids_to_delete) {
        if (server_to_client_entity_map.count(server_id)) {
            Entity client_entity_to_kill = server_to_client_entity_map.at(server_id);
            registry.kill_entity(client_entity_to_kill);
            server_to_client_entity_map.erase(server_id);
        }
    }
}

void ClientOnlineGameScene::update(GameEngine& engine, float delta_time)
{
    for (const auto& snapshot : _snapshots_to_process) {
        applySnapshot(engine.getRegistry(), snapshot);
    }
    _snapshots_to_process.clear();
    engine.getRegistry().run_systems(engine, delta_time);
}


void ClientOnlineGameScene::render(GameEngine& gameEngine)
{
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

        if (i < sprites.size() && sprites[i]) {
            item.type = RenderType::SPRITE;
            item.z_index = sprites[i]->z_index;
            item.sprite_data = &(*sprites[i]);
            renderables.push_back(item);
        } else if (i < rects.size() && rects[i] && item.hitbox) { 
            item.type = RenderType::RECTANGLE;
            item.z_index = 50;
            item.rect_data = &(*rects[i]);
            renderables.push_back(item);
        } else if (i < circles.size() && circles[i]) {
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
