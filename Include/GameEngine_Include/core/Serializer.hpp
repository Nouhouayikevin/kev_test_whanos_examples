#pragma once
#include <vector>
#include <string>
#include "../Registry.hpp"
#include "../../GameEngine_Include/core/Serializer.hpp"
#include "../../All/Component.hpp"
#include <stdexcept>
#include <iostream>

constexpr uint32_t MAGIC_NUMBER = 0x53415645;
constexpr uint8_t SAVE_VERSION = 1;

enum class ComponentTypeID : uint8_t {
    Position = 1,
    Velocity = 2,
    Sprite = 3,
    Health = 4,
    PlayerTag = 5,
    Controllable = 6,
    EnemyTag = 7,
    Damage = 8,
    Shooter = 9,
    AI = 10,
    Animation = 11,
    ParallaxLayer = 12,
    Hitbox = 13,
    // On peut en ajouter d'autres ici au besoin...
};


template<typename T>
void write_to_buffer(std::vector<char>& buffer, const T& value) {
    const char* bytes = reinterpret_cast<const char*>(&value);
    buffer.insert(buffer.end(), bytes, bytes + sizeof(T));
}

// Helper pour écrire une string
inline void write_string_to_buffer(std::vector<char>& buffer, const std::string& str) {
    uint16_t length = static_cast<uint16_t>(str.size());
    write_to_buffer(buffer, length);
    buffer.insert(buffer.end(), str.begin(), str.end());
}

// Helper pour lire des données simples depuis le buffer
template<typename T>
T read_from_buffer(const std::vector<char>& buffer, size_t& offset) {
    if (offset + sizeof(T) > buffer.size()) {
        throw std::runtime_error("Buffer underflow while reading data.");
    }
    T value;
    std::copy(buffer.begin() + offset, buffer.begin() + offset + sizeof(T), reinterpret_cast<char*>(&value));
    offset += sizeof(T);
    return value;
}

// Helper pour lire une string
inline std::string read_string_from_buffer(const std::vector<char>& buffer, size_t& offset) {
    uint16_t length = read_from_buffer<uint16_t>(buffer, offset);
    if (offset + length > buffer.size()) {
        throw std::runtime_error("Buffer underflow while reading string.");
    }
    std::string str(buffer.begin() + offset, buffer.begin() + offset + length);
    offset += length;
    return str;
}

namespace Serializer {
    inline std::vector<char> pack(Registry& registry) {
        std::vector<char> buffer;

        // 1. Écrire l'en-tête du fichier
        write_to_buffer(buffer, MAGIC_NUMBER);
        write_to_buffer(buffer, SAVE_VERSION);
        
        // On récupère toutes les entités qui ont le badge "Serializable"
        auto& serializables = registry.get_components<Component::Core::Serializable>();
        std::vector<Entity> entities_to_save;
        for(size_t i = 0; i < serializables.size(); ++i) {
            if (serializables[i]) {
                entities_to_save.push_back(Entity(i, &registry));
            }
        }

        // 2. Écrire le nombre d'entités
        write_to_buffer<uint16_t>(buffer, entities_to_save.size());

        // 3. Boucler sur chaque entité à sauvegarder
        for (Entity& e : entities_to_save) {
            // Écrire l'ID de l'entité
            write_to_buffer<size_t>(buffer, static_cast<size_t>(e));

            // On va compter les composants et les écrire plus tard
            size_t component_count_pos = buffer.size();
            write_to_buffer<uint8_t>(buffer, 0); // Placeholder pour le compteur
            uint8_t component_count = 0;

            // --- Sauvegarde des composants ---
            // On teste chaque type de composant qu'on a décidé de sauvegarder
            
            if (e.has_component<Component::Core::Position>()) {
                write_to_buffer(buffer, ComponentTypeID::Position);
                auto& comp = e.get_component<Component::Core::Position>();
                write_to_buffer(buffer, comp);
                component_count++;
            }
            if (e.has_component<Component::Core::Velocity>()) {
                write_to_buffer(buffer, ComponentTypeID::Velocity);
                auto& comp = e.get_component<Component::Core::Velocity>();
                write_to_buffer(buffer, comp);
                component_count++;
            }
            if (e.has_component<Component::Graphics::Sprite>()) {
                write_to_buffer(buffer, ComponentTypeID::Sprite);
                auto& comp = e.get_component<Component::Graphics::Sprite>();
                write_to_buffer(buffer, comp);
                component_count++;
            }
            if (e.has_component<Component::Gameplay::Health>()) {
                write_to_buffer(buffer, ComponentTypeID::Health);
                auto& comp = e.get_component<Component::Gameplay::Health>();
                write_to_buffer(buffer, comp);
                component_count++;
            }
            if (e.has_component<Component::Gameplay::PlayerTag>()) {
                write_to_buffer(buffer, ComponentTypeID::PlayerTag);
                auto& comp = e.get_component<Component::Gameplay::PlayerTag>();
                write_to_buffer(buffer, comp);
                component_count++;
            }
            if (e.has_component<Component::Gameplay::Controllable>()) {
                write_to_buffer(buffer, ComponentTypeID::Controllable);
                auto& comp = e.get_component<Component::Gameplay::Controllable>();
                write_to_buffer(buffer, comp);
                component_count++;
            }
            if (e.has_component<Component::Gameplay::EnemyTag>()) {
                write_to_buffer(buffer, ComponentTypeID::EnemyTag);
                auto& comp = e.get_component<Component::Gameplay::EnemyTag>();
                write_string_to_buffer(buffer, comp.type);
                component_count++;
            }
            // EnemyTag contains std::string -> don't raw serialize it
            if (e.has_component<Component::Gameplay::Damage>()) {
                write_to_buffer(buffer, ComponentTypeID::Damage);
                auto& comp = e.get_component<Component::Gameplay::Damage>();
                write_to_buffer(buffer, comp);
                component_count++;
            }
            if (e.has_component<Component::Gameplay::Shooter>()) {
                write_to_buffer(buffer, ComponentTypeID::Shooter);
                auto& comp = e.get_component<Component::Gameplay::Shooter>();
                write_to_buffer(buffer, comp);
                component_count++;
            }
            if (e.has_component<Component::Gameplay::AI>()) {
                write_to_buffer(buffer, ComponentTypeID::AI);
                auto& comp = e.get_component<Component::Gameplay::AI>();
                write_to_buffer(buffer, comp);
                component_count++;
            }
            if (e.has_component<Component::Graphics::Animation>()) {
                write_to_buffer(buffer, ComponentTypeID::Animation);
                auto& comp = e.get_component<Component::Graphics::Animation>();
                write_to_buffer(buffer, comp);
                component_count++;
            }
            if (e.has_component<Component::Gameplay::ParallaxLayer>()) {
                write_to_buffer(buffer, ComponentTypeID::ParallaxLayer);
                auto& comp = e.get_component<Component::Gameplay::ParallaxLayer>();
                write_to_buffer(buffer, comp);
                component_count++;
            }
            if (e.has_component<Component::Core::Hitbox>()) {
                write_to_buffer(buffer, ComponentTypeID::Hitbox);
                auto& comp = e.get_component<Component::Core::Hitbox>();
                write_to_buffer(buffer, comp);
                component_count++;
            }
            
            
            // On revient en arrière pour écrire le bon nombre de composants
            *reinterpret_cast<uint8_t*>(&buffer[component_count_pos]) = component_count;
        }
        
        return buffer;
    }


    inline void unpack(const std::vector<char>& buffer, Registry& registry) {
        size_t offset = 0;

        // 1. Lire et valider l'en-tête
        uint32_t magic = read_from_buffer<uint32_t>(buffer, offset);
        if (magic != MAGIC_NUMBER) throw std::runtime_error("Invalid save file format.");
        
        uint8_t version = read_from_buffer<uint8_t>(buffer, offset);
        if (version != SAVE_VERSION) throw std::runtime_error("Incompatible save file version.");

        // 2. Préparation du Registry : on efface TOUT pour repartir d'un état propre
        // Cela évite les interactions fantômes avec des entités non sauvegardées
        registry.clear();

        // 3. Lire le nombre d'entités
        uint16_t entity_count = read_from_buffer<uint16_t>(buffer, offset);

        // 4. Boucler pour recréer chaque entité
        for (uint16_t i = 0; i < entity_count; ++i) {
            size_t entity_id = read_from_buffer<size_t>(buffer, offset);
            uint8_t component_count = read_from_buffer<uint8_t>(buffer, offset);

            // On recrée l'entité avec EXACTEMENT le même ID.
            // C'est CRUCIAL. Il faut peut-être une fonction spéciale dans le Registry.
            Entity e = registry.spawn_entity_with_id(entity_id);
            
            // On lui remet son badge de sérialisation
            e.add_component<Component::Core::Serializable>(Component::Core::Serializable{});

            for (uint8_t j = 0; j < component_count; ++j) {
                ComponentTypeID type_id = read_from_buffer<ComponentTypeID>(buffer, offset);
                
                switch (type_id) {
                    case ComponentTypeID::Position: {
                        auto comp = read_from_buffer<Component::Core::Position>(buffer, offset);
                        e.add_component<Component::Core::Position>(std::move(comp));
                        break;
                    }
                    case ComponentTypeID::Velocity: {
                        auto comp = read_from_buffer<Component::Core::Velocity>(buffer, offset);
                        e.add_component<Component::Core::Velocity>(std::move(comp));
                        break;
                    }
                    case ComponentTypeID::Sprite: {
                        auto comp = read_from_buffer<Component::Graphics::Sprite>(buffer, offset);
                        e.add_component<Component::Graphics::Sprite>(std::move(comp));
                        break;
                    }
                    case ComponentTypeID::Health: {
                        auto comp = read_from_buffer<Component::Gameplay::Health>(buffer, offset);
                        e.add_component<Component::Gameplay::Health>(std::move(comp));
                        break;
                    }
                    case ComponentTypeID::PlayerTag: {
                        auto comp = read_from_buffer<Component::Gameplay::PlayerTag>(buffer, offset);
                        e.add_component<Component::Gameplay::PlayerTag>(std::move(comp));
                        break;
                    }
                    case ComponentTypeID::Controllable: {
                        auto comp = read_from_buffer<Component::Gameplay::Controllable>(buffer, offset);
                        e.add_component<Component::Gameplay::Controllable>(std::move(comp));
                        break;
                    }
                    case ComponentTypeID::EnemyTag: {
                        std::string type = read_string_from_buffer(buffer, offset);
                        e.add_component<Component::Gameplay::EnemyTag>(Component::Gameplay::EnemyTag(type));
                        break;
                    }
                    case ComponentTypeID::Damage: {
                        auto comp = read_from_buffer<Component::Gameplay::Damage>(buffer, offset);
                        e.add_component<Component::Gameplay::Damage>(std::move(comp));
                        break;
                    }
                    case ComponentTypeID::Shooter: {
                        auto comp = read_from_buffer<Component::Gameplay::Shooter>(buffer, offset);
                        e.add_component<Component::Gameplay::Shooter>(std::move(comp));
                        break;
                    }
                    case ComponentTypeID::AI: {
                        auto comp = read_from_buffer<Component::Gameplay::AI>(buffer, offset);
                        e.add_component<Component::Gameplay::AI>(std::move(comp));
                        break;
                    }
                    case ComponentTypeID::Animation: {
                        auto comp = read_from_buffer<Component::Graphics::Animation>(buffer, offset);
                        e.add_component<Component::Graphics::Animation>(std::move(comp));
                        break;
                    }
                    case ComponentTypeID::ParallaxLayer: {
                        auto comp = read_from_buffer<Component::Gameplay::ParallaxLayer>(buffer, offset);
                        e.add_component<Component::Gameplay::ParallaxLayer>(std::move(comp));
                        break;
                    }
                    case ComponentTypeID::Hitbox: {
                        auto comp = read_from_buffer<Component::Core::Hitbox>(buffer, offset);
                        e.add_component<Component::Core::Hitbox>(std::move(comp));
                        break;
                    }
                    
                    default:
                        throw std::runtime_error("Unknown component type ID in save file.");
                }
            }
        }
        
        // Post-processing : réinitialiser les armes des shooters
        // car les entités "weapon" ne sont pas sérialisées (elles contiennent des std::string)
        auto& shooters = registry.get_components<Component::Gameplay::Shooter>();
        for (size_t i = 0; i < shooters.size(); ++i) {
            if (shooters[i]) {
                shooters[i]->current_weapon_entity = -1; // Force le PlayerShootingSystem à recréer l'arme
            }
        }
    }
}