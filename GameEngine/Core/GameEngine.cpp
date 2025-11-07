/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** GameEngine.cpp
*/

#include "../../Include/GameEngine_Include/core/GameEngine.hpp"
#include "../../Include/All/managers/SceneManager.hpp"

GameEngine::GameEngine() : _window_module(nullptr), _input_module(nullptr), _resource_module(), _level_manager(*this) {
    _scene_manager = std::make_unique<SceneManager>();
    _registries["default"];
    _scripting_manager = std::make_unique<ScriptingManager>(*this);
    std::cout << "GameEngine created." << std::endl;
}

GameEngine::~GameEngine() {
    std::cout << "GameEngine destroyed." << std::endl;
    shutdown();
}

void GameEngine::setWindow(std::unique_ptr<IWindow> _window_mod) {
    _window_module = std::move(_window_mod);
}

void GameEngine::setInputManager(std::unique_ptr<IInputManager> _input_mod) {
    _input_module = std::move(_input_mod);
}

void GameEngine::setNetwork(std::unique_ptr<INetwork> network) {
    _network_module = std::move(network);
}

IWindow& GameEngine::getWindowManager() {
    if (!_window_module) {
        throw std::runtime_error("Window module not set");
    }
    return *_window_module;
}

IInputManager& GameEngine::getInputManager() {
    if (!_input_module) {
        throw std::runtime_error("Input module not set");
    }
    return *_input_module;
}

ResourceManager& GameEngine::getResourceManager() {
    return _resource_module;
}

void GameEngine::initialize() {
    register_component_factories();

    try {
        std::cout << "Engine is loading global assets..." << std::endl;
        this->register_component_factories();
        _replay_manager = std::make_unique<ReplayManager>(*this);
        this->load_archetypes("Config/archetypes.json");
        std::cout << "Archetypes loaded successfully." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "FATAL ERROR during engine initialization: " << e.what() << std::endl;
    }
    std::cout << "GameEngine initialized successfully." << std::endl;
}

void GameEngine::shutdown() {
    if (_window_module && _window_module->isOpen()) {
        _window_module->close();
    }
    _window_module.reset();
    _input_module.reset();
}

Clock& GameEngine::getClock() {
    return _clock_module;
}

INetwork& GameEngine::getNetworkManager() {
    if (!_network_module) {
        throw std::runtime_error("Network module not set");
    }
    return *_network_module;
}

SceneManager& GameEngine::getSceneManager() {
    return *_scene_manager;
}