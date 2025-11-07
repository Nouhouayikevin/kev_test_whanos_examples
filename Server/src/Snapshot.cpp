#include "../include/Snapshot.hpp"
#include <cstring>
#include <iostream>

std::vector<uint8_t> Snapshot::serialize() const {
    std::vector<uint8_t> data;
    
    // Calculate total size needed
    size_t totalSize = calculateSize();
    data.reserve(totalSize);
    
    
    
    // Header: tick (4 bytes, little endian)
    data.push_back(static_cast<uint8_t>(tick & 0xFF));
    data.push_back(static_cast<uint8_t>((tick >> 8) & 0xFF));
    data.push_back(static_cast<uint8_t>((tick >> 16) & 0xFF));
    data.push_back(static_cast<uint8_t>((tick >> 24) & 0xFF));
    
    // Entity count (2 bytes, little endian)
    // uint16_t entity_count = static_cast<uint16_t>(entities.size());
    // data.push_back(static_cast<uint8_t>(entity_count & 0xFF));
    // data.push_back(static_cast<uint8_t>((entity_count >> 8) & 0xFF));
    
    // Serialize each entity
    for (const auto& entity : entities) {
        // Entity ID (4 bytes, little endian)
        data.push_back(static_cast<uint8_t>(entity.id & 0xFF));
        data.push_back(static_cast<uint8_t>((entity.id >> 8) & 0xFF));
        data.push_back(static_cast<uint8_t>((entity.id >> 16) & 0xFF));
        data.push_back(static_cast<uint8_t>((entity.id >> 24) & 0xFF));

        // Component count (1 byte)
        data.push_back(entity.component_count);
        
        // Serialize each component
        for (const auto& comp : entity.components) {
            // Component type (1 byte)
            data.push_back(static_cast<uint8_t>(comp.type));
            
            // Component data (type-specific)
            switch (comp.type) {
                case ComponentType::POSITION:
                    data.push_back(static_cast<uint8_t>(comp.pos.x_q & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.pos.x_q >> 8) & 0xFF));
                    data.push_back(static_cast<uint8_t>(comp.pos.y_q & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.pos.y_q >> 8) & 0xFF));
                    break;
                    
                case ComponentType::VELOCITY:
                    data.push_back(static_cast<uint8_t>(comp.vel.x_q & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.vel.x_q >> 8) & 0xFF));
                    data.push_back(static_cast<uint8_t>(comp.vel.y_q & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.vel.y_q >> 8) & 0xFF));
                    break;
                    
                case ComponentType::SCALE:
                    data.push_back(static_cast<uint8_t>(comp.scale.x_q & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.scale.x_q >> 8) & 0xFF));
                    data.push_back(static_cast<uint8_t>(comp.scale.y_q & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.scale.y_q >> 8) & 0xFF));
                    break;
                    
                case ComponentType::ROTATION:
                    data.push_back(static_cast<uint8_t>(comp.rotation.angle & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.rotation.angle >> 8) & 0xFF));
                    break;
                    
                case ComponentType::HITBOX:
                    data.push_back(static_cast<uint8_t>(comp.hitbox.w_q & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.hitbox.w_q >> 8) & 0xFF));
                    data.push_back(static_cast<uint8_t>(comp.hitbox.h_q & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.hitbox.h_q >> 8) & 0xFF));
                    break;
                    
                case ComponentType::SPRITE:
                    data.push_back(static_cast<uint8_t>(comp.sprite.texture_id & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.sprite.texture_id >> 8) & 0xFF));
                    data.push_back(static_cast<uint8_t>(comp.sprite.z_index & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.sprite.z_index >> 8) & 0xFF));
                    data.push_back(static_cast<uint8_t>(comp.sprite.frame_width & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.sprite.frame_width >> 8) & 0xFF));
                    data.push_back(static_cast<uint8_t>(comp.sprite.frame_height & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.sprite.frame_height >> 8) & 0xFF));
                    break;
                    
                case ComponentType::ANIMATION:
                    data.push_back(static_cast<uint8_t>(comp.animation.start_frame & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.animation.start_frame >> 8) & 0xFF));
                    data.push_back(static_cast<uint8_t>(comp.animation.num_frames & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.animation.num_frames >> 8) & 0xFF));
                    data.push_back(static_cast<uint8_t>(comp.animation.frame_duration & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.animation.frame_duration >> 8) & 0xFF));
                    data.push_back(static_cast<uint8_t>(comp.animation.loops & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.animation.loops >> 8) & 0xFF));
                    break;
                    
                case ComponentType::HEALTH:
                    data.push_back(static_cast<uint8_t>(comp.health.hp & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.health.hp >> 8) & 0xFF));
                    break;
                    
                case ComponentType::HEALTH_BAR:
                    data.push_back(static_cast<uint8_t>(comp.health_bar.owner & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.health_bar.owner >> 8) & 0xFF));
                    data.push_back(static_cast<uint8_t>(comp.health_bar.offset_x & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.health_bar.offset_x >> 8) & 0xFF));
                    data.push_back(static_cast<uint8_t>(comp.health_bar.offset_y & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.health_bar.offset_y >> 8) & 0xFF));
                    break;
                    
                case ComponentType::RECTANGLE_SHAPE:
                    data.push_back(static_cast<uint8_t>(comp.rect_shape.x_q & 0xFF));  // R
                    data.push_back(static_cast<uint8_t>((comp.rect_shape.x_q >> 8) & 0xFF));
                    data.push_back(static_cast<uint8_t>(comp.rect_shape.y_q & 0xFF));  // G
                    data.push_back(static_cast<uint8_t>((comp.rect_shape.y_q >> 8) & 0xFF));
                    data.push_back(static_cast<uint8_t>(comp.rect_shape.w_q & 0xFF));  // B
                    data.push_back(static_cast<uint8_t>((comp.rect_shape.w_q >> 8) & 0xFF));
                    data.push_back(static_cast<uint8_t>(comp.rect_shape.h_q & 0xFF));  // A
                    data.push_back(static_cast<uint8_t>((comp.rect_shape.h_q >> 8) & 0xFF));
                    break;
                    
                case ComponentType::CIRCLE_SHAPE:
                    data.push_back(static_cast<uint8_t>(comp.circle_shape.radius & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.circle_shape.radius >> 8) & 0xFF));
                    data.push_back(static_cast<uint8_t>(comp.circle_shape.outline_thickness & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.circle_shape.outline_thickness >> 8) & 0xFF));
                    break;
                    
                case ComponentType::PLAYER_TAG:
                    data.push_back(static_cast<uint8_t>(comp.player_tag.id_player & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.player_tag.id_player >> 8) & 0xFF));
                    break;
                    
                case ComponentType::DAMAGE:
                    data.push_back(static_cast<uint8_t>(comp.damage.damage & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.damage.damage >> 8) & 0xFF));
                    break;
                    
                case ComponentType::ENEMY_TAG:
                    data.push_back(static_cast<uint8_t>(comp.enemy_tag.type & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.enemy_tag.type >> 8) & 0xFF));
                    break;
                    
                case ComponentType::MISSILE_TAG:
                    data.push_back(static_cast<uint8_t>(comp.missile_tag.owner & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.missile_tag.owner >> 8) & 0xFF));
                    break;
                    
                case ComponentType::PARALLAX_LAYER:
                    data.push_back(static_cast<uint8_t>(comp.parallax_layer.scroll_speed & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.parallax_layer.scroll_speed >> 8) & 0xFF));
                    break;
                    
                case ComponentType::SCORE:
                    data.push_back(static_cast<uint8_t>(comp.score.value & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.score.value >> 8) & 0xFF));
                    break;
                    
                case ComponentType::DEAD:
                    data.push_back(static_cast<uint8_t>(comp.dead.dead & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.dead.dead >> 8) & 0xFF));
                    break;
                    
                case ComponentType::SOUND_EFFECT:
                    data.push_back(static_cast<uint8_t>(comp.sound_effect.sound_id & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.sound_effect.sound_id >> 8) & 0xFF));
                    data.push_back(static_cast<uint8_t>(comp.sound_effect.to_play & 0xFF));
                    data.push_back(static_cast<uint8_t>((comp.sound_effect.to_play >> 8) & 0xFF));
                    break;
                    
                case ComponentType::CONTROLLABLE:
                    // No data for controllable component
                    break;
                    
                case ComponentType::AI:
                    // AI component not serialized in snapshot (server-side only)
                    break;
                    
                default:
                    std::cerr << "Warning: Unknown component type " 
                              << static_cast<int>(comp.type) << std::endl;
                    break;
            }
        }
    }
    
    return data;
}



size_t Snapshot::calculateSize() const {
    size_t size = 0;
    
    // Header: tick (4 bytes) + entity_count (2 bytes)
    size += 6;
    
    // For each entity
    for (const auto& entity : entities) {
        // entity_id (4 bytes) + component_count (1 byte)
        size += 5;
        
        // For each component
        for (const auto& comp : entity.components) {
            // component_type (1 byte)
            size += 1;
            
            // Component data size (type-specific)
            switch (comp.type) {
                case ComponentType::POSITION:
                case ComponentType::VELOCITY:
                case ComponentType::SCALE:
                case ComponentType::HITBOX:
                    size += 4; // 2x int16_t = 4 bytes
                    break;
                    
                case ComponentType::ROTATION:
                case ComponentType::HEALTH:
                case ComponentType::PLAYER_TAG:
                case ComponentType::DAMAGE:
                case ComponentType::ENEMY_TAG:
                case ComponentType::MISSILE_TAG:
                case ComponentType::PARALLAX_LAYER:
                case ComponentType::SCORE:
                case ComponentType::DEAD:
                    size += 2; // 1x int16_t = 2 bytes
                    break;
                    
                case ComponentType::SPRITE:
                    size += 8; // 4x int16_t = 8 bytes
                    break;
                    
                case ComponentType::ANIMATION:
                case ComponentType::RECTANGLE_SHAPE:
                case ComponentType::CIRCLE_SHAPE:
                case ComponentType::SOUND_EFFECT:
                    size += 8; // 4x int16_t = 8 bytes
                    break;
                    
                case ComponentType::HEALTH_BAR:
                    size += 6; // 3x int16_t = 6 bytes
                    break;
                    
                case ComponentType::CONTROLLABLE:
                case ComponentType::AI:
                    size += 0; // No data
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    return size;
}
