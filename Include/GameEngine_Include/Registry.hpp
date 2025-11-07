/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** Registry.hpp
*/


#pragma once

#include "../All/interfaces/ISystem.hpp"
#include "sparse_array.hpp"
#include <unordered_map>
#include <typeindex>
#include <any>
#include <vector>
#include <functional>
#include <stdexcept>

#include <cstddef>
#include <optional>
#include <string>
#include <utility>
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>



class GameEngine;
class Registry;

class Entity {
public:
    explicit Entity(std::size_t id, Registry* registry = nullptr) {
        _id = id;
        _registry = registry;
    }

    operator std::size_t() const {
        return _id;
    }

    bool operator==(const Entity& other) const {
        return _id == other._id;
    }

    template <typename Component>
    void add_component(Component&& c);

    template <typename Component>
    Component& get_component();

    template <typename Component>
    bool has_component() const;

    template <typename Component>
    void remove_component();



    void set_name(const std::string& name);
    std::optional<std::string> get_name();

private:
    std::size_t _id;
    Registry* _registry;
};

namespace std {
    template <>
    struct hash<Entity> {
        std::size_t operator()(const Entity& e) const {
            return std::hash<std::size_t>()(static_cast<std::size_t>(e));
        }
    };
}



class Registry {

public:
    Registry() {
        this->register_component<Component::Core::Position>();
        this->register_component<Component::Core::Velocity>();
        this->register_component<Component::Core::Scale>();
        this->register_component<Component::Core::Rotation>();
        this->register_component<Component::Core::Hitbox>();
        this->register_component<Component::Core::Serializable>();
        this->register_component<Component::Graphics::Text>();

        this->register_component<Component::Gameplay::PlayerTag>();
        this->register_component<Component::Gameplay::Damage>();
        this->register_component<Component::Gameplay::Controllable>();
        this->register_component<Component::Gameplay::EnemyTag>();
        this->register_component<Component::Gameplay::MissileTag>();
        this->register_component<Component::Gameplay::ParallaxLayer>();
        this->register_component<Component::Gameplay::AI>();
        this->register_component<Component::Gameplay::Health>();
        this->register_component<Component::Gameplay::Score>();
        this->register_component<Component::Gameplay::Dead>();
        this->register_component<Component::Gameplay::Shooter>();
        this->register_component<Component::Gameplay::Weapon>();
        this->register_component<Component::Gameplay::PowerUp>();
        this->register_component<Component::Gameplay::TimedLifespan>();
        this->register_component<Component::Gameplay::Force>();
        this->register_component<Component::Gameplay::ForceTag>();
        
        this->register_component<Component::Graphics::Sprite>();
        this->register_component<Component::Graphics::Animation>();
        this->register_component<Component::Graphics::RectangleShape>();
        this->register_component<Component::Graphics::CircleShape>();
        this->register_component<Component::Graphics::UIText>();
        this->register_component<Component::Graphics::ButtonStateComponent>();
        this->register_component<Component::Graphics::Clickable>();
        this->register_component<Component::Graphics::Action>();

        this->register_component<Component::UI::HealthBar>();
        this->register_component<Component::UI::Clickable>();
        this->register_component<Component::UI::Action>();
        
        this->register_component<Component::Audio::SoundEffect>();

        this->register_component<Component::Gameplay::Script>();
        this->register_component<Component::Gameplay::BossTag>();
        this->register_component<Component::Audio::Music>();
        
        _running = true;
    }
    
    ~Registry() {
        stop_all_systems();
    }
    // --- Gestion des Entités ---
    Entity spawn_entity() {
        if (!_free_entities.empty()) {
            Entity e = _free_entities.back();
            _free_entities.pop_back();
            return e;
        }
        return Entity(_next_entity_id++, this);
    }
    
    void kill_entity(Entity const &e) {
        if (_entity_to_name.count(e)) {
            std::string name_to_remove = _entity_to_name.at(e);
            _name_to_entity.erase(name_to_remove);
            _entity_to_name.erase(e);
        }

        for (auto const& [type, eraser_func] : _component_erasers) {
            eraser_func(e);
        }

        _free_entities.push_back(e);
    }

    size_t get_entity_count() const {
        return _next_entity_id;
    }

    template <typename Component>
    sparse_array<Component>& register_component() {
        std::type_index const key(typeid(Component));
        
        if (_components_arrays.find(key) == _components_arrays.end()) {
            // Créer le sparse_array comme avant
            _components_arrays.emplace(key, sparse_array<Component>());

            // *** LE SEUL AJOUT ***
            // On stocke une lambda qui sait comment effacer un composant de CE type.
            // On capture 'this' pour pouvoir appeler get_components.
            _component_erasers[key] = [this](Entity e) {
                this->get_components<Component>().erase(e);
            };
        }
        return std::any_cast<sparse_array<Component>&>(_components_arrays.at(key));
    }

    template <typename Component>
    sparse_array<Component>&
    get_components() {
    
        std::type_index const key(typeid(Component));
        if (_components_arrays.find(key) == _components_arrays.end()) {
            throw std::runtime_error("Component not registered");
        }
        return std::any_cast<sparse_array<Component>&>(_components_arrays.at(key));
    }

    template <typename Component>
    const sparse_array<Component>&
    get_components() const {
    
        std::type_index const key(typeid(Component));
        if (_components_arrays.find(key) == _components_arrays.end()) {
            throw std::runtime_error("Component not registered");
        }
        return std::any_cast<const sparse_array<Component>&>(_components_arrays.at(key));
    }

    template <typename Component>
    typename sparse_array<Component>::reference_type
    add_component(Entity const &to, Component &&c) {
    
        sparse_array<Component>& array = get_components<Component>();
        return array.insert_at(to, std::forward<Component>(c));
    }

    template <typename Component, typename... Params>
    typename sparse_array<Component>::reference_type
    emplace_component(Entity const &to, Params &&... p) {
    
        sparse_array<Component>& array = get_components<Component>();
        return array.emplace_at(to, std::forward<Params>(p)...);
    }

    template <typename Component>
    void remove_component(Entity const &from) {
        sparse_array<Component>& array = get_components<Component>();
        array.erase(from);
    }

    template <typename T>
    void add_system(const std::string& systemGroupName, T&& system, float update_frequency_hz = 60.0f, bool threaded = true) {
        std::lock_guard<std::mutex> lock(_system_mutex);
        
        auto& group_data = _system_groups[systemGroupName];

        if constexpr (std::is_base_of_v<ISystem, std::decay_t<T>>) {
            group_data.systems.push_back(std::make_unique<std::decay_t<T>>(std::forward<T>(system)));
        } else {
            group_data.systems.push_back(std::make_unique<SystemWrapper<T>>(std::forward<T>(system)));
        }
        
        // Si c'est le premier système de ce groupe
        if (!group_data.thread_running && !group_data.is_main_thread) {
            group_data.update_frequency = update_frequency_hz;
            group_data.is_main_thread = !threaded;
            
            // Démarrer un thread uniquement si threaded = true
            if (threaded) {
                group_data.thread_running = true;
                
                std::cout << "[Registry] Starting thread for system group '" << systemGroupName 
                          << "' at " << update_frequency_hz << " Hz" << std::endl;
                
                group_data.thread = std::thread(&Registry::system_loop, this, systemGroupName);
            } else {
                std::cout << "[Registry] System group '" << systemGroupName 
                          << "' will run in MAIN THREAD at " << update_frequency_hz << " Hz" << std::endl;
            }
        }
    }
    
    // Fonction pour exécuter manuellement les systèmes du thread principal
    void run_main_thread_systems(GameEngine& engine, float dt) {
        std::lock_guard<std::mutex> lock(_system_mutex);
        
        int systems_executed = 0;
        for (auto& [group_name, group_data] : _system_groups) {
            // Exécuter uniquement les groupes marqués comme "main thread"
            if (group_data.is_main_thread && !group_data.paused) {
                for (auto& system : group_data.systems) {
                    try {
                        system->run(engine, dt);
                        systems_executed++;
                    } catch (const std::exception& e) {
                        std::cerr << "[Registry] Exception in main thread system (group '" 
                                  << group_name << "'): " << e.what() << std::endl;
                    }
                }
            }
        }
        
        if (systems_executed == 0) {
            std::cerr << "[Registry] WARNING: No main thread systems were executed!" << std::endl;
        }
    }
    
    // Fonction pour exécuter UN SEUL groupe spécifique (pour séparer update et render)
    void run_system_group(const std::string& group_name, GameEngine& engine, float dt) {
        std::lock_guard<std::mutex> lock(_system_mutex);
        
        if (_system_groups.count(group_name) == 0) {
            std::cerr << "[Registry] ERROR: System group '" << group_name << "' does not exist!" << std::endl;
            return;
        }
        
        auto& group_data = _system_groups[group_name];
        
        if (!group_data.is_main_thread) {
            std::cerr << "[Registry] WARNING: Trying to run threaded group '" << group_name << "' manually!" << std::endl;
            return;
        }
        
        if (group_data.paused) {
            return;
        }
        
        for (auto& system : group_data.systems) {
            try {
                system->run(engine, dt);
            } catch (const std::exception& e) {
                std::cerr << "[Registry] Exception in system (group '" << group_name << "'): " << e.what() << std::endl;
            }
        }
    }
    
    // Fonction pour arrêter tous les threads de systèmes
    void stop_all_systems() {
        std::cout << "[Registry] Stopping all system threads..." << std::endl;
        _running = false;
        
        for (auto& [name, group_data] : _system_groups) {
            if (group_data.thread.joinable()) {
                std::cout << "[Registry] Waiting for thread '" << name << "' to finish..." << std::endl;
                group_data.thread.join();
            }
        }
        std::cout << "[Registry] All system threads stopped." << std::endl;
    }
    
    // Fonction pour mettre en pause/reprendre un groupe de systèmes
    void pause_system_group(const std::string& systemGroupName) {
        std::lock_guard<std::mutex> lock(_system_mutex);
        if (_system_groups.count(systemGroupName)) {
            _system_groups[systemGroupName].paused = true;
            std::cout << "[Registry] System group '" << systemGroupName << "' paused." << std::endl;
        }
    }
    
    void resume_system_group(const std::string& systemGroupName) {
        std::lock_guard<std::mutex> lock(_system_mutex);
        if (_system_groups.count(systemGroupName)) {
            _system_groups[systemGroupName].paused = false;
            std::cout << "[Registry] System group '" << systemGroupName << "' resumed." << std::endl;
        }
    }
    
    // Fonction pour changer la fréquence de mise à jour d'un groupe
    void set_system_frequency(const std::string& systemGroupName, float frequency_hz) {
        std::lock_guard<std::mutex> lock(_system_mutex);
        if (_system_groups.count(systemGroupName)) {
            _system_groups[systemGroupName].update_frequency = frequency_hz;
            std::cout << "[Registry] System group '" << systemGroupName 
                      << "' frequency changed to " << frequency_hz << " Hz" << std::endl;
        }
    }
    
    // Setter pour la référence au GameEngine (nécessaire pour les threads)
    void set_engine_reference(GameEngine* engine) {
        _engine_ref = engine;
    }

    void set_name(Entity const &e, const std::string& name) {
        if (_name_to_entity.count(name)) {
            throw std::runtime_error("Entity name '" + name + "' already exists.");
        }
        
        if (_entity_to_name.count(e)) {
            std::string old_name = _entity_to_name.at(e);
            _name_to_entity.erase(old_name);
        }

        _name_to_entity.insert_or_assign(name, e);
        _entity_to_name.insert_or_assign(e, name);
    }

    std::optional<Entity> get_entity_by_name(const std::string& name) {
        if (_name_to_entity.count(name)) {
            return _name_to_entity.at(name);
        }
        return std::nullopt;
    }

    std::optional<std::string> get_name(Entity const &e) {
        if (_entity_to_name.count(e)) {
            return _entity_to_name.at(e);
        }
        return std::nullopt;
    }

    void clear() {
        // Efface toutes les entités mais garde les types de composants enregistrés
        // On tue toutes les entités existantes
        for (size_t i = 0; i < _next_entity_id; ++i) {
            Entity e(i, this);
            // Efface tous les composants de cette entité
            for (auto const& [type, eraser_func] : _component_erasers) {
                eraser_func(e);
            }
        }
        
        _name_to_entity.clear();
        _entity_to_name.clear();
        _free_entities.clear();
        _next_entity_id = 0;
    }

    Entity spawn_entity_with_id(size_t id) {
        // Attention: cette fonction est "dangereuse" et ne doit être utilisée que
        // par le système de chargement.
        _next_entity_id = std::max(_next_entity_id, id + 1);
        return Entity(id, this);
    }


private:

    template <typename Func>
    class SystemWrapper : public ISystem {
    public:
        SystemWrapper(Func&& func) : _func(std::forward<Func>(func)) {}
        void run(GameEngine& engine, float dt) override {
            _func(engine, dt);
        }
    private:
        Func _func;
    };
    
    // Structure pour gérer un groupe de systèmes avec son thread
    struct SystemGroup {
        std::vector<std::unique_ptr<ISystem>> systems;
        std::thread thread;
        float update_frequency = 60.0f;  // Hz
        bool thread_running = false;
        bool paused = false;
        bool is_main_thread = false;  // Si true, s'exécute dans le thread principal
    };
    
    // Boucle d'exécution d'un groupe de systèmes dans son propre thread
    void system_loop(const std::string& group_name) {
        std::cout << "[Registry] Thread for '" << group_name << "' started." << std::endl;
        
        while (_running) {
            auto start_time = std::chrono::high_resolution_clock::now();
            
            // Récupérer les données du groupe de manière thread-safe
            float frequency;
            bool is_paused;
            {
                std::lock_guard<std::mutex> lock(_system_mutex);
                if (!_system_groups.count(group_name)) break;
                
                auto& group_data = _system_groups[group_name];
                frequency = group_data.update_frequency;
                is_paused = group_data.paused;
            }
            
            // Si le groupe est en pause, attendre
            if (is_paused) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            
            // Calculer le delta time
            float dt = 1.0f / frequency;
            
            // Exécuter tous les systèmes du groupe
            {
                std::lock_guard<std::mutex> lock(_system_mutex);
                if (_system_groups.count(group_name)) {
                    auto& group_data = _system_groups[group_name];
                    
                    // Note: On doit passer une référence au GameEngine
                    // Pour l'instant on utilise un pointeur global (à améliorer)
                    if (_engine_ref) {
                        for (auto& system : group_data.systems) {
                            try {
                                system->run(*_engine_ref, dt);
                            } catch (const std::exception& e) {
                                std::cerr << "[Registry] Exception in system (group '" 
                                          << group_name << "'): " << e.what() << std::endl;
                            }
                        }
                    }
                }
            }
            
            // Calculer le temps de sleep pour maintenir la fréquence
            auto end_time = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
            auto target_duration = std::chrono::microseconds(static_cast<long long>(1000000.0f / frequency));
            
            if (elapsed < target_duration) {
                std::this_thread::sleep_for(target_duration - elapsed);
            }
        }
        
        std::cout << "[Registry] Thread for '" << group_name << "' stopped." << std::endl;
    }

    
    std::unordered_map<std::type_index, std::function<void(Entity)>> _component_erasers;
    std::unordered_map<std::string, Entity> _name_to_entity;
    std::unordered_map<Entity, std::string> _entity_to_name;
    std::unordered_map<std::type_index, std::any> _components_arrays;
    std::vector<std::unique_ptr<ISystem>> _systems;
    std::vector<std::unique_ptr<ISystem>> _update_system;
    std::vector<std::unique_ptr<ISystem>> _render_system;
    std::unordered_map<std::string, SystemGroup> _system_groups;
    std::vector<Entity> _free_entities;
    size_t _next_entity_id = 0;
    
    // Gestion multi-threading
    std::atomic<bool> _running;
    std::mutex _system_mutex;
    GameEngine* _engine_ref = nullptr;  // Référence au moteur (doit être set par le GameEngine)
};



template <typename Component>
inline void Entity::add_component(Component&& c) {
    if (!_registry)throw std::runtime_error("You can't use an empty (null) registry through entity");
    _registry->add_component<Component>(*this, std::forward<Component>(c));
}

template <typename Component>
inline Component& Entity::get_component() {
    if (!_registry)throw std::runtime_error("You can't use an empty (null) registry through entity");
    return  _registry->get_components<Component>()[*this].value();
}

template <typename Component>
inline bool Entity::has_component() const {
    
    if (!_registry)throw std::runtime_error("You can't use an empty (null) registry through entity");
    return  _registry->get_components<Component>().has_component(*this);
}

template <typename Component>
inline void Entity::remove_component() {
    if (!_registry)throw std::runtime_error("You can't use an empty (null) registry through entity");
    _registry->remove_component<Component>(*this);
}

inline void Entity::set_name(const std::string& name) {
    if (!_registry)throw std::runtime_error("You can't use an empty (null) registry through entity");
    _registry->set_name(*this, name);
}

inline std::optional<std::string> Entity::get_name() {
    if (!_registry)throw std::runtime_error("You can't use an empty (null) registry through entity");
    return _registry->get_name(*this);
}

