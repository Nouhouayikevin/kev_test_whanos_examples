#pragma once
#include <string>
#include <fstream>
class GameEngine;

// Notre nouvel enum pour définir l'état
enum class ReplayState {
    INACTIVE,
    RECORDING,
    PLAYING
};

class ReplayManager {
public:
    ReplayManager(GameEngine& engine);
    ~ReplayManager();

    // Les fonctions sont vides pour l'instant, ce sont juste des boutons
    void start_recording(const std::string& filepath);
    void stop_recording();
    void start_playback(const std::string& filepath);
    void stop_playback();
    
    // La fonction qui sera appelée à chaque frame
    void update(float dt);

    // Pour savoir ce que le manager est en train de faire
    ReplayState get_state() const;

private:
    void record_current_frame();
    void play_next_frame();

    std::ofstream _record_file; // Fichier pour l'écriture
    std::ifstream _play_file;   // Fichier pour la lecture

    GameEngine& _game_engine;
    ReplayState _state = ReplayState::INACTIVE;
    std::string _filepath;

    const float _frame_duration = 1.0f / 60.0f; // On lit le replay à 60 FPS
    float _playback_timer = 0.0f;
};