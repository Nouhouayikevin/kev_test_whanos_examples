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
#include "../../Include/All/Scenes/TestGameScene.hpp"
#include "../../Include/All/Component.hpp"
#include "../../Include/All/interfaces/ISprite.hpp"
#include "../../Include/All/interfaces/ISound.hpp"
#include "../../Include/All/systems/ForceSystem.hpp"
#include "../../Include/All/systems/ForceControlSystem.hpp"
#include "../../Include/All/systems/ForceCollisionSystem.hpp"
#include "../../Include/All/systems/ForceInitSystem.hpp"
#include "../../Include/All/systems/OffScreenDeathSystem.hpp"


void TestGameScene::init(GameEngine& gameSceneEngine)
{
    std::cout << "Initializing TestGameScene..." << std::endl;

    gameSceneEngine.createRegistry(group);
    auto &registry = gameSceneEngine.getRegistry(group);
    auto &levelManager = gameSceneEngine.getLevelManager();

    // Les composants sont maintenant enregistrés automatiquement dans le constructeur du Registry

    // Définir le registre par défaut pour les appels Lua
    gameSceneEngine.getScriptingManager().setDefaultRegisterGroup(group);
    
    // ========== GROUPE UPDATE (LOGIQUE) - Thread principal ==========
    registry.add_system("logic", PlayerControlSystem(group), 60.0f, true);
    registry.add_system("logic", PlayerShootingSystem(group), 60.0f, true);
    registry.add_system("logic", AISystem(group), 60.0f, true);
    registry.add_system("logic", EnemyShootingSystem(group), 60.0f, true);
    registry.add_system("logic", WeaponSystem(group), 60.0f, true);
    registry.add_system("logic", PowerUpSystem(group), 60.0f, true);
    registry.add_system("logic", LifespanSystem(group), 60.0f, true);
    registry.add_system("logic", DeathSystem(group), 60.0f, true);
    registry.add_system("logic", SaveLoadSystem(group), 60.0f, true);
    
    registry.add_system("physic", MovementSystem(group), 60.0f, true);
    registry.add_system("physic", CollisionSystem(group), 60.0f, true);
    registry.add_system("physic", OffScreenDeathSystem(group), 60.0f, true);
    
    registry.add_system("force", ForceInitSystem(group), 60.0f, true);
    registry.add_system("force", ForceSystem(group), 60.0f, true);
    registry.add_system("force", ForceControlSystem(group), 60.0f, true);
    registry.add_system("force", ForceCollisionSystem(group), 60.0f, true);
    
    registry.add_system("music", SoundSystem(group), 60.0f, true);
    registry.add_system("music", MusicSystem(group), 60.0f, true);
    
    // ========== GROUPE RENDER (AFFICHAGE) - Thread principal ==========
    // Ces systèmes seront appelés dans render() APRÈS window.clear()
    registry.add_system("render", AnimationSystem(group), 60.0f, false);
    registry.add_system("render", ParallaxSystem(group), 60.0f, false);
    registry.add_system("render", HealthBarSystem(group), 60.0f, false);
    registry.add_system("render", RenderSystem(group), 160.0f, false);  // RenderSystem EN DERNIER

    registry.add_system("script", ScriptSystem(group), 60.0f, true);
    std::cout << "✅ Systems configured: 'update' group (21 systems) + 'render' group (4 systems)" << std::endl;

    levelManager.set_register_group(group);
    levelManager.load("Config/levels/level_1.json");  // 🎮 TEST DU BOSS
    // levelManager.load("Config/levels/test_powerups.json");  // 🎮 TEST DES POWERUPS
    // levelManager.load("Config/levels/test_projectiles_only.json");
    
    std::cout << "TestGameScene Initialized." << std::endl;
}

void TestGameScene::update(GameEngine& gameSceneEngine, float delta_time)
{
    auto &registry = gameSceneEngine.getRegistry(group);
    auto& replay_manager = gameSceneEngine.getReplayManager();

    if (replay_manager.get_state() == ReplayState::PLAYING) {
        // En mode lecture, on ne fait TOURNER AUCUN SYSTÈME de logique.
        // On laisse le ReplayManager mettre à jour le Registry.
        replay_manager.update(delta_time);

    } else {
        // En mode normal ou enregistrement, on exécute toute la logique de jeu.
        gameSceneEngine.getLevelManager().update(delta_time, group);

        // Si on est en mode enregistrement, on dit au manager de prendre sa photo
        // MAINTENANT, *après* que tous les systèmes de logique ont tourné.
        if (replay_manager.get_state() == ReplayState::RECORDING) {
            replay_manager.update(delta_time);
        }
    }
    
}

void TestGameScene::render(GameEngine& gameEngine, float delta_time)
{
    auto &registry = gameEngine.getRegistry(group);

    registry.run_system_group("render", gameEngine, delta_time);

}

