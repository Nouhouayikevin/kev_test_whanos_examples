#include "../include/Room.hpp"
#include "../../Include/All/Scenes/Online_GameScene_server.hpp"
#include "../include/SnapshotHelper.hpp"
#include <chrono>
#include <thread>
#include <iostream>

void Room::addClient(Client &client) {
    std::lock_guard<std::mutex> lock(clientsMutex_);
    if (isFull()) {
        std::cerr << "Room " << roomId_ << " is full, cannot add client" << std::endl;
        return;
    }
    client.setRoomId(id());
    clients_.push_back(client);
    std::cout << "Client added to room " << roomId_ 
              << " (Players: " << clients_.size() << "/" << kMaxClients << ")" << std::endl;
    
    // Si le jeu est déjà initialisé, crée le joueur immédiatement
    if (gameSceneInitialized_) {
        uint32_t player_id = static_cast<uint32_t>(clients_.size());
        Snapshot playerSnap = game.init_entity_player(gameSceneEngine_, player_id, tickCounter_);
        // TODO: Envoyer le snapshot initial au nouveau client
        std::cout << "Player " << player_id << " spawned in room " << roomId_ << std::endl;
    }
}

void Room::broadcast(const Snapshot &snapshot) {
    std::lock_guard<std::mutex> lock(clientsMutex_);
    std::cout << "snapshot envoyé\n";
    snapshotSender_.sendToAll(clients_, snapshot);
}

void Room::processInputs() {
    // Les inputs dans inputQueue_ sont automatiquement accessibles
    // via game._inputQueue (qui est une référence vers inputQueue_)
    // Le système de mouvement dans OnlineGameScene_server::init() 
    // va les traiter directement, donc on ne fait rien ici.
}

void Room::run() {
    std::cout << "Room " << roomId_ << " starting..." << std::endl;
    running_ = true;
    
    // 1. Initialise le moteur de jeu et la scène
    std::cout << "Initializing game scene for room " << roomId_ << std::endl;
    game.init(gameSceneEngine_);
    gameSceneInitialized_ = true;
    
    // 2. Crée un joueur pour chaque client déjà connecté
    {
        std::lock_guard<std::mutex> lock(clientsMutex_);
        std::cout << "Creating " << clients_.size() << " players in room " << roomId_ << std::endl;
        
        for (size_t i = 0; i < clients_.size(); i++) {
            uint32_t player_id = static_cast<uint32_t>(i + 1);
            Snapshot playerSnap = game.init_entity_player(gameSceneEngine_, player_id, tickCounter_);
            
            // TODO: Envoyer le snapshot initial au client spécifique
            // snapshotSender_.sendTo(clients_[i], playerSnap);
            std::cout << "Player " << player_id << " spawned in room " << roomId_ << std::endl;
        }
    }
    
    std::cout << "Room " << roomId_ << " game loop starting at " << (1000.0f / tickMs_) << " FPS" << std::endl;
    
    // 3. Boucle de jeu principale (~60 FPS)
    auto lastTick = std::chrono::steady_clock::now();
    
    while (running_) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTick);
        float dt = elapsed.count() / 1000.0f;
        
        // Processus les inputs (déjà dans inputQueue_)
        processInputs();
        
        // Update la logique de jeu (systèmes ECS)
        // Les systèmes lisent _inputQueue et modifient les composants
        game.update(gameSceneEngine_, dt);
        
        // Crée un snapshot de l'état actuel du jeu
        // Récupère toutes les entités du registry
        auto& registry = gameSceneEngine_.getRegistry();
        
        // Récupère toutes les entités en itérant sur les composants Position
        // (on suppose que toutes les entités visibles ont une Position)
        auto& positions = registry.get_components<Component::Core::Position>();
        
        Snapshot snap;
        snap.tick = tickCounter_;
        
        // Itère sur toutes les entités qui ont une position
        for (size_t i = 0; i < positions.size(); i++) {
            if (positions[i]) {
                // Crée une entité avec cet index
                Entity entity(i, &registry);
                
                // Crée le snapshot de cette entité
                EntitySnapshot entSnap = SnapshotHelper::createEntitySnapshot(entity, registry);
                snap.entities.push_back(entSnap);
            }
        }
        
        // Broadcast à tous les clients de cette room
        broadcast(snap);
        
        tickCounter_++;
        lastTick = now;
        
        // Maintient ~60 FPS
        std::this_thread::sleep_for(std::chrono::milliseconds(tickMs_));
    }
    
    std::cout << "Room " << roomId_ << " stopped after " << tickCounter_ << " ticks" << std::endl;
}
