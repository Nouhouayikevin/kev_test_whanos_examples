#include "../../Include/GameEngine_Include/managers/ReplayManager.hpp"
#include "../../Include/GameEngine_Include/core/GameEngine.hpp"
#include "../../Include/GameEngine_Include/core/Serializer.hpp"
#include <iostream>

ReplayManager::ReplayManager(GameEngine& engine) 
    : _game_engine(engine) {}

ReplayManager::~ReplayManager() {
    // S'assurer que les fichiers sont bien fermés à la fin
    if (_record_file.is_open()) _record_file.close();
    if (_play_file.is_open()) _play_file.close();
}

void ReplayManager::start_recording(const std::string& filepath) {
    if (_state != ReplayState::INACTIVE) return;

    _filepath = filepath;
    _record_file.open(_filepath, std::ios::binary);

    if (!_record_file.is_open()) {
        std::cerr << "ReplayManager Error: Could not open file for recording: " << _filepath << std::endl;
        return;
    }
    
    // On écrit l'en-tête une seule fois
    _record_file.write(reinterpret_cast<const char*>(&MAGIC_NUMBER), sizeof(MAGIC_NUMBER));
    _record_file.write(reinterpret_cast<const char*>(&SAVE_VERSION), sizeof(SAVE_VERSION));

    _state = ReplayState::RECORDING;
    std::cout << "ReplayManager: Started recording." << std::endl;
}

void ReplayManager::stop_recording() {
    if (_state != ReplayState::RECORDING) return;
    
    _record_file.close();
    _state = ReplayState::INACTIVE;
    std::cout << "ReplayManager: Stopped recording." << std::endl;
}

void ReplayManager::start_playback(const std::string& filepath) {
    if (_state != ReplayState::INACTIVE) return;

    _filepath = filepath;
    _play_file.open(_filepath, std::ios::binary);

    if (!_play_file.is_open()) {
        std::cerr << "ReplayManager Error: Could not open file for playback: " << _filepath << std::endl;
        return;
    }

    // On lit et valide l'en-tête
    uint32_t magic;
    uint8_t version;
    _play_file.read(reinterpret_cast<char*>(&magic), sizeof(magic));
    _play_file.read(reinterpret_cast<char*>(&version), sizeof(version));

    if (magic != MAGIC_NUMBER || version != SAVE_VERSION) {
        std::cerr << "ReplayManager Error: Invalid or incompatible replay file." << std::endl;
        _play_file.close();
        return;
    }

    _playback_timer = 0.0f;
    _state = ReplayState::PLAYING;
    std::cout << "ReplayManager: Started playback." << std::endl;
}

void ReplayManager::stop_playback() {
    if (_state != ReplayState::PLAYING) return;
    
    _play_file.close();
    _state = ReplayState::INACTIVE;
    std::cout << "ReplayManager: Stopped playback." << std::endl;
    
    // Crucial : On recharge le niveau pour que le jeu puisse reprendre normalement
    _game_engine.getLevelManager().restart_current();
}

void ReplayManager::update(float dt) {
    switch (_state) {
        case ReplayState::RECORDING:
            record_current_frame();
            break;
        case ReplayState::PLAYING:
            _playback_timer += dt;
            // On affiche autant de frames que nécessaire pour rattraper le temps
            while (_playback_timer >= _frame_duration) {
                play_next_frame();
                _playback_timer -= _frame_duration;
            }
            break;
        case ReplayState::INACTIVE:
            // Ne rien faire
            break;
    }
}

ReplayState ReplayManager::get_state() const {
    return _state;
}

void ReplayManager::record_current_frame() {
    if (!_record_file.is_open()) return;

    // On prend la photo de la frame actuelle (uniquement le registre du jeu)
    std::vector<char> snapshot = Serializer::pack(_game_engine.getRegistry("default"));

    // On écrit la taille du snapshot, puis le snapshot lui-même
    uint32_t size = snapshot.size();
    _record_file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    _record_file.write(snapshot.data(), size);
}

void ReplayManager::play_next_frame() {
    if (!_play_file.is_open() || _play_file.peek() == EOF) {
        stop_playback();
        return;
    }

    // On lit la taille du prochain snapshot
    uint32_t size;
    _play_file.read(reinterpret_cast<char*>(&size), sizeof(size));

    // On lit le snapshot dans un buffer
    std::vector<char> buffer(size);
    _play_file.read(buffer.data(), size);

    // On développe la photo et on l'applique au registre du jeu
    Serializer::unpack(buffer, _game_engine.getRegistry("default"));
}