/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Mon, Sep, 2025                                                     *
 * Title           - G-CPP-500-COT-5-1-rtype-24                                         *
 * Description     -                                                                    *
 *     Protocole                                                                        *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       ▄▀▀█▄▄▄▄  ▄▀▀▄▀▀▀▄  ▄▀▀█▀▄    ▄▀▀▀█▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▄▄▄▄   ▄▀▀▄ ▄▄             *
 *      ▐  ▄▀   ▐ █   █   █ █   █  █  █    █  ▐ ▐  ▄▀   ▐ █ █    ▌ █  █   ▄▀            *
 *        █▄▄▄▄▄  ▐  █▀▀▀▀  ▐   █  ▐  ▐   █       █▄▄▄▄▄  ▐ █      ▐  █▄▄▄█             *
 *        █    ▌     █          █        █        █    ▌    █         █   █             *
 *       ▄▀▄▄▄▄    ▄▀        ▄▀▀▀▀▀▄   ▄▀        ▄▀▄▄▄▄    ▄▀▄▄▄▄▀   ▄▀  ▄▀             *
 *       █    ▐   █         █       █ █          █    ▐   █     ▐   █   █               *
 *       ▐        ▐         ▐       ▐ ▐          ▐        ▐         ▐   ▐               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */
#include <cstring>
#include <arpa/inet.h>
#include "../include/Protocole.hpp"
#include <iostream>

Protocol::Protocol() {
}

std::vector<uint8_t> Protocol::encode_input(uint8_t input_bits) {
    std::vector<uint8_t> packet;
    packet.push_back(input_bits);
    return packet;
}

Snapshot Protocol::decodeSnapshot(const std::vector<uint8_t>& data)
{
    Snapshot snap;
    size_t i = 0;

    // --- Fonction d'aide pour lire des valeurs Little Endian ---
    auto read_u32 = [&]() -> uint32_t {
        if (i + 4 > data.size()) return 0;
        uint32_t val = (uint32_t)data[i] |
                       (uint32_t)data[i+1] << 8 |
                       (uint32_t)data[i+2] << 16 |
                       (uint32_t)data[i+3] << 24;
        i += 4;
        return val;
    };
    auto read_u16 = [&]() -> int16_t {
        if (i + 2 > data.size()) return 0;
        uint16_t val = static_cast<uint16_t>(data[i]) |
                       (static_cast<uint16_t>(data[i+1]) << 8);
        i += 2;
        return static_cast<int16_t>(val);
    };
    auto read_u8 = [&]() -> uint8_t {
        if (i + 1 > data.size()) return 0;
        return data[i++];
    };

    // ==========================================================
    // DÉBUT DE LA DÉSÉRIALISATION
    // ==========================================================
    
    // 1. Lire le tick (4 octets, Little Endian)
    snap.tick = read_u32();

    // 2. Le serveur NE SÉRIALISE PAS le nombre d'entités.
    //    On doit donc lire les entités jusqu'à la fin du paquet.
    while (i < data.size()) {
        EntitySnapshot entity;

        // 3. Lire l'ID de l'entité (4 octets, Little Endian)
        entity.id = read_u32();

        // 4. Lire le nombre de composants (1 octet)
        entity.component_count = read_u8();

        // 5. Lire chaque composant
        for (uint8_t c = 0; c < entity.component_count; ++c) {
            Components comp;

            // 5a. Lire le type du composant (1 octet)
            comp.type = static_cast<ComponentType>(read_u8());

            // 5b. Lire les données du composant en fonction de son type
            switch (comp.type) {
                // Composants avec 2x int16_t (4 octets)
                case ComponentType::POSITION:
                    comp.pos.x_q = read_u16();
                    comp.pos.y_q = read_u16();
                    break;
                case ComponentType::VELOCITY:
                    comp.vel.x_q = read_u16();
                    comp.vel.y_q = read_u16();
                    break;
                case ComponentType::SCALE:
                    comp.scale.x_q = read_u16();
                    comp.scale.y_q = read_u16();
                    break;
                case ComponentType::HITBOX:
                    comp.hitbox.w_q = read_u16();
                    comp.hitbox.h_q = read_u16();
                    break;

                // Composants avec 1x int16_t (2 octets)
                case ComponentType::ROTATION:       comp.rotation.angle = read_u16(); break;
                case ComponentType::HEALTH:         comp.health.hp = read_u16(); break;
                case ComponentType::PLAYER_TAG:     comp.player_tag.id_player = read_u16(); break;
                case ComponentType::DAMAGE:         comp.damage.damage = read_u16(); break;
                case ComponentType::ENEMY_TAG:      comp.enemy_tag.type = read_u16(); break;
                case ComponentType::MISSILE_TAG:    comp.missile_tag.owner = read_u16(); break;
                case ComponentType::PARALLAX_LAYER: comp.parallax_layer.scroll_speed = read_u16(); break;
                case ComponentType::SCORE:          comp.score.value = read_u16(); break;
                case ComponentType::DEAD:           comp.dead.dead = read_u16(); break;
                
                // Composants avec 4x int16_t (8 octets)
                case ComponentType::SPRITE:
                    comp.sprite.texture_id = read_u16();
                    comp.sprite.z_index = read_u16();
                    comp.sprite.frame_width = read_u16();
                    comp.sprite.frame_height = read_u16();
                    break;
                case ComponentType::ANIMATION:
                    comp.animation.start_frame = read_u16();
                    comp.animation.num_frames = read_u16();
                    comp.animation.frame_duration = read_u16();
                    comp.animation.loops = read_u16();
                    break;
                case ComponentType::RECTANGLE_SHAPE:
                    comp.rect_shape.x_q = read_u16();
                    comp.rect_shape.y_q = read_u16();
                    comp.rect_shape.w_q = read_u16();
                    comp.rect_shape.h_q = read_u16();
                    break;

                // Autres tailles
                case ComponentType::HEALTH_BAR:
                    comp.health_bar.owner = read_u16();
                    comp.health_bar.offset_x = read_u16();
                    comp.health_bar.offset_y = read_u16();
                    break;
                case ComponentType::CIRCLE_SHAPE:
                    comp.circle_shape.radius = read_u16();
                    comp.circle_shape.outline_thickness = read_u16();
                    break;
                case ComponentType::SOUND_EFFECT:
                    comp.sound_effect.sound_id = read_u16();
                    comp.sound_effect.to_play = read_u16();
                    break;

                // Composants sans données
                case ComponentType::CONTROLLABLE:
                case ComponentType::AI:
                    break;

                default:
                    std::cerr << "Warning: decodeSnapshot a rencontré un type de composant inconnu: " 
                              << static_cast<int>(comp.type) << ". Arrêt de la lecture du paquet." << std::endl;
                    return snap; // On retourne ce qu'on a pu lire jusqu'à présent
            }
            entity.components.push_back(comp);
        }
        snap.entities.push_back(entity);
    }
    
    return snap;
}