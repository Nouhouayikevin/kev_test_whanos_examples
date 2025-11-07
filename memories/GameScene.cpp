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
#define  DELTA_TIME(dt) (dt * 1)


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
    gameSceneEngine.getRegistry().register_component<Component::Gameplay::PlayerTag>();
    gameSceneEngine.getRegistry().register_component<Component::Gameplay::Controllable>();
    gameSceneEngine.getRegistry().register_component<Component::Gameplay::EnemyTag>();
    gameSceneEngine.getRegistry().register_component<Component::Gameplay::MissileTag>();
    gameSceneEngine.getRegistry().register_component<Component::Gameplay::ParallaxLayer>();
    gameSceneEngine.getRegistry().register_component<Component::Graphics::Sprite>();
    gameSceneEngine.getRegistry().register_component<Component::Graphics::Animation>();
    gameSceneEngine.getRegistry().register_component<Component::Audio::SoundEffect>();

    
    
    
    
    Entity player = gameSceneEngine.getRegistry().spawn_entity();
    gameSceneEngine.getRegistry().add_component(player, Component::Core::Position(100.0f, 300.0f));
    gameSceneEngine.getRegistry().add_component(player, Component::Core::Velocity(0.0f, 0.0f));
    gameSceneEngine.getRegistry().add_component(player, Component::Graphics::Sprite(player_texture_id, 33, 22, 10, 0, 0));
    gameSceneEngine.getRegistry().add_component(player, Component::Graphics::Animation(0, 5, 10.0f, true));
    gameSceneEngine.getRegistry().add_component(player, Component::Gameplay::PlayerTag(0)); 
    gameSceneEngine.getRegistry().add_component(player, Component::Gameplay::Controllable{});

    
    Entity bg1 = gameSceneEngine.getRegistry().spawn_entity();
    gameSceneEngine.getRegistry().add_component(bg1, Component::Core::Position(0, 0));
    gameSceneEngine.getRegistry().add_component(bg1, Component::Core::Velocity(-30.0f, 0));
    gameSceneEngine.getRegistry().add_component(bg1, Component::Graphics::Sprite(bg1_texture_id, 1920, 1080, 0, 0, 0));
    
    gameSceneEngine.getRegistry().add_component(bg1, Component::Gameplay::ParallaxLayer(-30.0f));

    Entity bg2 = gameSceneEngine.getRegistry().spawn_entity();
    gameSceneEngine.getRegistry().add_component(bg2, Component::Core::Position(0, 0));
    gameSceneEngine.getRegistry().add_component(bg2, Component::Core::Velocity(-60.0f, 0));
    gameSceneEngine.getRegistry().add_component(bg2, Component::Graphics::Sprite(bg2_texture_id, 1920, 1080, 1, 0, 0));
    
    gameSceneEngine.getRegistry().add_component(bg2, Component::Gameplay::ParallaxLayer(-60.0f));
    
    Entity bg1_bis = gameSceneEngine.getRegistry().spawn_entity();
    gameSceneEngine.getRegistry().add_component(bg1_bis, Component::Core::Position(1920, 0));
    gameSceneEngine.getRegistry().add_component(bg1_bis, Component::Core::Velocity(-30.0f, 0));
    gameSceneEngine.getRegistry().add_component(bg1_bis, Component::Graphics::Sprite(bg1_texture_id, 1920, 1080, 1, 0, 0));
    
    gameSceneEngine.getRegistry().add_component(bg1_bis, Component::Gameplay::ParallaxLayer(-30.0f));

    Entity bg2_bis = gameSceneEngine.getRegistry().spawn_entity();
    gameSceneEngine.getRegistry().add_component(bg2_bis, Component::Core::Position(1920, 0));
    gameSceneEngine.getRegistry().add_component(bg2_bis, Component::Core::Velocity(-60.0f, 0));
    gameSceneEngine.getRegistry().add_component(bg2_bis, Component::Graphics::Sprite(bg2_texture_id, 1920, 1080, 1, 0, 0));
    
    gameSceneEngine.getRegistry().add_component(bg2_bis, Component::Gameplay::ParallaxLayer(-60.0f));


    
    
    
    

    
    gameSceneEngine.getRegistry().add_system([](GameEngine& gameEng ,float dt) {
        auto& velocities = gameEng.getRegistry().get_components<Component::Core::Velocity>();
        auto const& controllables = gameEng.getRegistry().get_components<Component::Gameplay::Controllable>();

        for (size_t i = 0; i < velocities.size() && i < controllables.size(); i++) {
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
                        gameEng.getRegistry().add_component(missile, Component::Core::Position(positions[i]->x + 50, positions[i]->y + 20)); 
                        gameEng.getRegistry().add_component(missile, Component::Core::Velocity(400.0f, 0.0f));
                        gameEng.getRegistry().add_component(missile, Component::Graphics::Sprite(missile_texture_id,10,10, 9, 0, 0));
                        gameEng.getRegistry().add_component(missile, Component::Gameplay::MissileTag(0)); 
                        gameEng.getRegistry().add_component(missile, Component::Audio::SoundEffect(laser_sound_id, true)); 
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
            if (positions[i] && velocities[i]) {
                if (positions[i]) { 

                    
                    
                    
                }
                positions[i]->x += velocities[i]->dx * DELTA_TIME(dt);
                positions[i]->y += velocities[i]->dy * DELTA_TIME(dt);
            }
        }
    });
    
    
    gameSceneEngine.getRegistry().add_system([](GameEngine& gameEng ,float dt) {
        auto& positions = gameEng.getRegistry().get_components<Component::Core::Position>();
        auto const& parallaxes = gameEng.getRegistry().get_components<Component::Gameplay::ParallaxLayer>();
        
        for (size_t i = 0; i < positions.size() && i < parallaxes.size(); i++) {
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
            if (animations[i] && sprites[i]) {
                auto& anim = *animations[i];
                auto& sprite = *sprites[i];

                anim.timer += DELTA_TIME(dt);

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
}

void GameScene::update(GameEngine& gameSceneEngine, float delta_time) {
    gameSceneEngine.getRegistry().run_systems(gameSceneEngine, delta_time);

    _enemy_spawn_timer -= DELTA_TIME(delta_time);
    if (_enemy_spawn_timer <= 0.0f) {
        Entity enemy = gameSceneEngine.getRegistry().spawn_entity();
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(50, 1000);
        float random_y = distrib(gen);

        gameSceneEngine.getRegistry().add_component(enemy, Component::Core::Position(1000, random_y));
        gameSceneEngine.getRegistry().add_component(enemy, Component::Core::Velocity(-100.0f, 0.0f)); 
        gameSceneEngine.getRegistry().add_component(enemy, Component::Graphics::Sprite(enemy_texture_id,34, 33, 10, 0, 0));
        
        gameSceneEngine.getRegistry().add_component(enemy, Component::Gameplay::EnemyTag("BydosSlave"));
        
        std::uniform_real_distribution<> time_distrib(1.0, 3.0);
        _enemy_spawn_timer = time_distrib(gen);
    }
}

void GameScene::render(GameEngine& gameSceneEngine) {
    ResourceManager& resources = gameSceneEngine.getResourceManager();

    struct RenderableEntity {
        bool has_animation;
        int z_index;
        Component::Core::Position position;
        Component::Graphics::Sprite sprite;
        RenderableEntity(
                        int has_animation,
                        int z_index,
                        Component::Core::Position& position,
                        Component::Graphics::Sprite& sprite) {
                            this->has_animation = has_animation, this->z_index =z_index, this->position = position, this->sprite = sprite;
                        }
    };

    std::vector<RenderableEntity> renderables;

    auto & sprites = gameSceneEngine.getRegistry().get_components<Component::Graphics::Sprite>();
    auto & positions = gameSceneEngine.getRegistry().get_components<Component::Core::Position>();
    auto & animations = gameSceneEngine.getRegistry().get_components<Component::Graphics::Animation>();
    
    for (size_t i = 0; i < sprites.size() && i < positions.size(); i++) {
        if (sprites[i] && positions[i]) {
            renderables.push_back(RenderableEntity(
                i < animations.size() && animations[i] ? true : false,
                sprites[i]->z_index,
                *positions[i],
                *sprites[i]
            ));
        }
    }

    std::sort(renderables.begin(), renderables.end(), [](const auto& a, const auto& b) {
        return a.z_index < b.z_index;
    });

    for (const auto& renderable : renderables) {
        ITexture& texture = resources.get_texture(renderable.sprite.texture_id);
        
        std::unique_ptr<ISprite> display_sprite = resources.createSprite();
        display_sprite->setTexture(texture);
        display_sprite->setPosition({renderable.position.x, renderable.position.y});

        if (renderable.has_animation) {
            display_sprite->setTextureRect({
                renderable.sprite.source_rect.x,
                renderable.sprite.source_rect.y,
                renderable.sprite.source_rect.width,
                renderable.sprite.source_rect.height
            });
        }
        
        gameSceneEngine.getWindowManager().draw(*display_sprite);
    }
}

