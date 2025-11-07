/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** SnapshotHelper.cpp - Implementation of snapshot helper functions
*/

#include "../include/SnapshotHelper.hpp"

EntitySnapshot SnapshotHelper::createEntitySnapshot(const Entity& entity, Registry& registry) {
    EntitySnapshot snapshot(static_cast<size_t>(entity));
    
    // Check and add each component type if it exists
    size_t entity_id = static_cast<size_t>(entity);
    
    // Position
    try {
        auto& positions = registry.get_components<Component::Core::Position>();
        if (entity_id < positions.size() && positions[entity_id]) {
            addPositionComponent(snapshot, *positions[entity_id]);
        }
    } catch (...) {}
    
    // Velocity
    try {
        auto& velocities = registry.get_components<Component::Core::Velocity>();
        if (entity_id < velocities.size() && velocities[entity_id]) {
            addVelocityComponent(snapshot, *velocities[entity_id]);
        }
    } catch (...) {}
    
    // Scale
    try {
        auto& scales = registry.get_components<Component::Core::Scale>();
        if (entity_id < scales.size() && scales[entity_id]) {
            addScaleComponent(snapshot, *scales[entity_id]);
        }
    } catch (...) {}
    
    // Rotation
    try {
        auto& rotations = registry.get_components<Component::Core::Rotation>();
        if (entity_id < rotations.size() && rotations[entity_id]) {
            addRotationComponent(snapshot, *rotations[entity_id]);
        }
    } catch (...) {}
    
    // Hitbox
    try {
        auto& hitboxes = registry.get_components<Component::Core::Hitbox>();
        if (entity_id < hitboxes.size() && hitboxes[entity_id]) {
            addHitboxComponent(snapshot, *hitboxes[entity_id]);
        }
    } catch (...) {}
    
    // Sprite
    try {
        auto& sprites = registry.get_components<Component::Graphics::Sprite>();
        if (entity_id < sprites.size() && sprites[entity_id]) {
            addSpriteComponent(snapshot, *sprites[entity_id]);
        }
    } catch (...) {}
    
    // Animation
    try {
        auto& anims = registry.get_components<Component::Graphics::Animation>();
        if (entity_id < anims.size() && anims[entity_id]) {
            addAnimationComponent(snapshot, *anims[entity_id]);
        }
    } catch (...) {}
    
    // Health
    try {
        auto& healths = registry.get_components<Component::Gameplay::Health>();
        if (entity_id < healths.size() && healths[entity_id]) {
            addHealthComponent(snapshot, *healths[entity_id]);
        }
    } catch (...) {}
    
    // Health Bar
    try {
        auto& healthBars = registry.get_components<Component::UI::HealthBar>();
        if (entity_id < healthBars.size() && healthBars[entity_id]) {
            addHealthBarComponent(snapshot, *healthBars[entity_id]);
        }
    } catch (...) {}
    
    // Rectangle Shape
    try {
        auto& rects = registry.get_components<Component::Graphics::RectangleShape>();
        if (entity_id < rects.size() && rects[entity_id]) {
            addRectangleShapeComponent(snapshot, *rects[entity_id]);
        }
    } catch (...) {}
    
    // Circle Shape
    try {
        auto& circles = registry.get_components<Component::Graphics::CircleShape>();
        if (entity_id < circles.size() && circles[entity_id]) {
            addCircleShapeComponent(snapshot, *circles[entity_id]);
        }
    } catch (...) {}
    
    // Player Tag
    try {
        auto& tags = registry.get_components<Component::Gameplay::PlayerTag>();
        if (entity_id < tags.size() && tags[entity_id]) {
            addPlayerTagComponent(snapshot, *tags[entity_id]);
        }
    } catch (...) {}
    
    // Damage
    try {
        auto& damages = registry.get_components<Component::Gameplay::Damage>();
        if (entity_id < damages.size() && damages[entity_id]) {
            addDamageComponent(snapshot, *damages[entity_id]);
        }
    } catch (...) {}
    
    // Enemy Tag
    try {
        auto& tags = registry.get_components<Component::Gameplay::EnemyTag>();
        if (entity_id < tags.size() && tags[entity_id]) {
            addEnemyTagComponent(snapshot, *tags[entity_id]);
        }
    } catch (...) {}
    
    // Missile Tag
    try {
        auto& tags = registry.get_components<Component::Gameplay::MissileTag>();
        if (entity_id < tags.size() && tags[entity_id]) {
            addMissileTagComponent(snapshot, *tags[entity_id]);
        }
    } catch (...) {}
    
    // Score
    try {
        auto& scores = registry.get_components<Component::Gameplay::Score>();
        if (entity_id < scores.size() && scores[entity_id]) {
            addScoreComponent(snapshot, *scores[entity_id]);
        }
    } catch (...) {}
    
    // Dead
    try {
        auto& deads = registry.get_components<Component::Gameplay::Dead>();
        if (entity_id < deads.size() && deads[entity_id]) {
            addDeadComponent(snapshot);
        }
    } catch (...) {}
    
    snapshot.component_count = static_cast<uint8_t>(snapshot.components.size());
    return snapshot;
}

Snapshot SnapshotHelper::createSnapshot(const std::vector<Entity>& entities, Registry& registry, uint32_t tick) {
    Snapshot snapshot;
    snapshot.tick = tick;
    
    for (const auto& entity : entities) {
        EntitySnapshot entitySnapshot = createEntitySnapshot(entity, registry);
        snapshot.entities.push_back(entitySnapshot);
    }
    
    return snapshot;
}

// Component converters implementation

void SnapshotHelper::addPositionComponent(EntitySnapshot& snapshot, const Component::Core::Position& pos) {
    Components comp(ComponentType::POSITION);
    comp.pos.x_q = static_cast<int16_t>(pos.x * QUANTIZATION_FACTOR);
    comp.pos.y_q = static_cast<int16_t>(pos.y * QUANTIZATION_FACTOR);
    snapshot.components.push_back(comp);
}

void SnapshotHelper::addVelocityComponent(EntitySnapshot& snapshot, const Component::Core::Velocity& vel) {
    Components comp(ComponentType::VELOCITY);
    comp.vel.x_q = static_cast<int16_t>(vel.dx * QUANTIZATION_FACTOR);
    comp.vel.y_q = static_cast<int16_t>(vel.dy * QUANTIZATION_FACTOR);
    snapshot.components.push_back(comp);
}

void SnapshotHelper::addScaleComponent(EntitySnapshot& snapshot, const Component::Core::Scale& scale) {
    Components comp(ComponentType::SCALE);
    comp.scale.x_q = static_cast<int16_t>(scale.width * QUANTIZATION_FACTOR);
    comp.scale.y_q = static_cast<int16_t>(scale.height * QUANTIZATION_FACTOR);
    snapshot.components.push_back(comp);
}

void SnapshotHelper::addRotationComponent(EntitySnapshot& snapshot, const Component::Core::Rotation& rot) {
    Components comp(ComponentType::ROTATION);
    comp.rotation.angle = static_cast<int16_t>(rot.angle * QUANTIZATION_FACTOR);
    snapshot.components.push_back(comp);
}

void SnapshotHelper::addHitboxComponent(EntitySnapshot& snapshot, const Component::Core::Hitbox& hitbox) {
    Components comp(ComponentType::HITBOX);
    comp.hitbox.w_q = static_cast<int16_t>(hitbox.width * QUANTIZATION_FACTOR);
    comp.hitbox.h_q = static_cast<int16_t>(hitbox.height * QUANTIZATION_FACTOR);
    snapshot.components.push_back(comp);
}

void SnapshotHelper::addSpriteComponent(EntitySnapshot& snapshot, const Component::Graphics::Sprite& sprite) {
    Components comp(ComponentType::SPRITE);
    comp.sprite.texture_id = static_cast<int16_t>(sprite.texture_id);
    comp.sprite.z_index = static_cast<int16_t>(sprite.z_index);
    comp.sprite.frame_width = static_cast<int16_t>(sprite.frame_width);
    comp.sprite.frame_height = static_cast<int16_t>(sprite.frame_height);
    snapshot.components.push_back(comp);
}

void SnapshotHelper::addAnimationComponent(EntitySnapshot& snapshot, const Component::Graphics::Animation& anim) {
    Components comp(ComponentType::ANIMATION);
    comp.animation.start_frame = static_cast<int16_t>(anim.start_frame);
    comp.animation.num_frames = static_cast<int16_t>(anim.num_frames);
    comp.animation.frame_duration = static_cast<int16_t>(anim.frame_duration * QUANTIZATION_FACTOR);
    comp.animation.loops = static_cast<int16_t>(anim.loops ? 1 : 0);
    snapshot.components.push_back(comp);
}

void SnapshotHelper::addHealthComponent(EntitySnapshot& snapshot, const Component::Gameplay::Health& health) {
    Components comp(ComponentType::HEALTH);
    comp.health.hp = static_cast<int16_t>(health.hp);
    snapshot.components.push_back(comp);
}

void SnapshotHelper::addHealthBarComponent(EntitySnapshot& snapshot, const Component::UI::HealthBar& healthBar) {
    Components comp(ComponentType::HEALTH_BAR);
    comp.health_bar.owner = static_cast<int16_t>(healthBar.owner);
    comp.health_bar.offset_x = static_cast<int16_t>(healthBar.offset_x * QUANTIZATION_FACTOR);
    comp.health_bar.offset_y = static_cast<int16_t>(healthBar.offset_y * QUANTIZATION_FACTOR);
    snapshot.components.push_back(comp);
}

void SnapshotHelper::addRectangleShapeComponent(EntitySnapshot& snapshot, const Component::Graphics::RectangleShape& rect) {
    Components comp(ComponentType::RECTANGLE_SHAPE);
    comp.rect_shape.x_q = static_cast<int16_t>(rect.fill_color.r);  // R
    comp.rect_shape.y_q = static_cast<int16_t>(rect.fill_color.g);  // G
    comp.rect_shape.w_q = static_cast<int16_t>(rect.fill_color.b);  // B
    comp.rect_shape.h_q = static_cast<int16_t>(rect.fill_color.a);  // A
    snapshot.components.push_back(comp);
}

void SnapshotHelper::addCircleShapeComponent(EntitySnapshot& snapshot, const Component::Graphics::CircleShape& circle) {
    Components comp(ComponentType::CIRCLE_SHAPE);
    comp.circle_shape.radius = static_cast<int16_t>(circle.radius * QUANTIZATION_FACTOR);
    comp.circle_shape.outline_thickness = 0; // Not used for now
    snapshot.components.push_back(comp);
}

void SnapshotHelper::addPlayerTagComponent(EntitySnapshot& snapshot, const Component::Gameplay::PlayerTag& tag) {
    Components comp(ComponentType::PLAYER_TAG);
    comp.player_tag.id_player = static_cast<int16_t>(tag.player_id);
    snapshot.components.push_back(comp);
}

void SnapshotHelper::addDamageComponent(EntitySnapshot& snapshot, const Component::Gameplay::Damage& damage) {
    Components comp(ComponentType::DAMAGE);
    comp.damage.damage = static_cast<int16_t>(damage.amount);
    snapshot.components.push_back(comp);
}

void SnapshotHelper::addEnemyTagComponent(EntitySnapshot& snapshot, const Component::Gameplay::EnemyTag& tag) {
    Components comp(ComponentType::ENEMY_TAG);
    comp.enemy_tag.type = 0; // Type could be extended later
    snapshot.components.push_back(comp);
}

void SnapshotHelper::addMissileTagComponent(EntitySnapshot& snapshot, const Component::Gameplay::MissileTag& tag) {
    Components comp(ComponentType::MISSILE_TAG);
    comp.missile_tag.owner = static_cast<int16_t>(tag.owner);
    snapshot.components.push_back(comp);
}

void SnapshotHelper::addScoreComponent(EntitySnapshot& snapshot, const Component::Gameplay::Score& score) {
    Components comp(ComponentType::SCORE);
    comp.score.value = static_cast<int16_t>(score.value);
    snapshot.components.push_back(comp);
}

void SnapshotHelper::addDeadComponent(EntitySnapshot& snapshot) {
    Components comp(ComponentType::DEAD);
    comp.dead.dead = 1;
    snapshot.components.push_back(comp);
}
