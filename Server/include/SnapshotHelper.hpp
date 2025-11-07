/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** SnapshotHelper.hpp - Helper functions for creating snapshots from entities
*/

#pragma once

#include "Snapshot.hpp"
#include "../../Include/GameEngine_Include/Registry.hpp"
#include "../../Include/All/Component.hpp"

class SnapshotHelper {
public:
    // Quantization factor for float to int16_t conversion (0.1 precision)
    static constexpr float QUANTIZATION_FACTOR = 10.0f;
    
    /**
     * @brief Create an EntitySnapshot from an Entity
     * @param entity The entity to convert
     * @param registry The registry containing the entity's components
     * @return EntitySnapshot with all serialized components
     */
    static EntitySnapshot createEntitySnapshot(const Entity& entity, Registry& registry);
    
    /**
     * @brief Create a full Snapshot from multiple entities
     * @param entities Vector of entities to include in the snapshot
     * @param registry The registry containing the entities' components
     * @param tick Current game tick
     * @return Complete snapshot ready for serialization
     */
    static Snapshot createSnapshot(const std::vector<Entity>& entities, Registry& registry, uint32_t tick = 0);
    
private:
    // Helper methods to convert specific components
    static void addPositionComponent(EntitySnapshot& snapshot, const Component::Core::Position& pos);
    static void addVelocityComponent(EntitySnapshot& snapshot, const Component::Core::Velocity& vel);
    static void addScaleComponent(EntitySnapshot& snapshot, const Component::Core::Scale& scale);
    static void addRotationComponent(EntitySnapshot& snapshot, const Component::Core::Rotation& rot);
    static void addHitboxComponent(EntitySnapshot& snapshot, const Component::Core::Hitbox& hitbox);
    static void addSpriteComponent(EntitySnapshot& snapshot, const Component::Graphics::Sprite& sprite);
    static void addAnimationComponent(EntitySnapshot& snapshot, const Component::Graphics::Animation& anim);
    static void addHealthComponent(EntitySnapshot& snapshot, const Component::Gameplay::Health& health);
    static void addHealthBarComponent(EntitySnapshot& snapshot, const Component::UI::HealthBar& healthBar);
    static void addRectangleShapeComponent(EntitySnapshot& snapshot, const Component::Graphics::RectangleShape& rect);
    static void addCircleShapeComponent(EntitySnapshot& snapshot, const Component::Graphics::CircleShape& circle);
    static void addPlayerTagComponent(EntitySnapshot& snapshot, const Component::Gameplay::PlayerTag& tag);
    static void addDamageComponent(EntitySnapshot& snapshot, const Component::Gameplay::Damage& damage);
    static void addEnemyTagComponent(EntitySnapshot& snapshot, const Component::Gameplay::EnemyTag& tag);
    static void addMissileTagComponent(EntitySnapshot& snapshot, const Component::Gameplay::MissileTag& tag);
    static void addScoreComponent(EntitySnapshot& snapshot, const Component::Gameplay::Score& score);
    static void addDeadComponent(EntitySnapshot& snapshot);
};
