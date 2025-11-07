/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Oct, 2025                                                     *
 * Title           - G-CPP-500-COT-5-1-rtype-24                                         *
 * Description     -                                                                    *
 *     OfflineApplication                                                               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _|           *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _|           *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_|           *
 *       _|        _|          _|        _|      _|        _|        _|    _|           *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _|           *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */


#include "../include/ServerOfflineApplication.hpp"
#include "../All/sfml/SfmlWindow.hpp"
#include "../All/sfml/SfmlInputManager.hpp"
#include "../All/sfml/SfmlResourceFactory.hpp"
#include "../All/Scenes/MenuScene.hpp"
#include "../../Include/All/Scenes/Online_GameScene_server.hpp"
#include "../All/Scenes/ConnectionScene.hpp"


ServerOfflineApplication::ServerOfflineApplication()
{
    std::cout << "ServerOfflineApplication created." << std::endl;
}

ServerOfflineApplication::~ServerOfflineApplication()
{
    std::cout << "ServerOfflineApplication destroyed." << std::endl;
}

void ServerOfflineApplication::initialize()
{
    std::cout << "Initializing ServerOfflineApplication..." << std::endl;
    auto window = std::make_unique<SfmlWindow>(1280, 720, "R-Type Offline");
    auto input = std::make_unique<SfmlInputManager>(*window);
    auto resource_factory = std::make_unique<SfmlResourceFactory>();

    engine.setWindow(std::move(window));
    engine.setInputManager(std::move(input));
    engine.getResourceManager().setResourceFactory(std::move(resource_factory));

    //engine.getSceneManager().add_scene("connection", std::make_shared<>());
    engine.getSceneManager().add_scene("server_online_game", std::make_shared<OnlineGameScene_server>(inputQueue_));
    engine.getSceneManager().switch_to_scene("server_online_game", engine);

    engine.initialize();
    std::cout << "ServerOfflineApplication initialized successfully." << std::endl;
}

void ServerOfflineApplication::run() {
    std::cout << "Starting offline game loop..." << std::endl;
    Clock& clock = engine.getClock();
    IWindow& window = engine.getWindowManager();
    IInputManager& input = engine.getInputManager();

    while (window.isOpen()) {
        float dt = clock.restart();
        input.update();
        if (input.isWindowClosed())
            window.close();
        engine.getSceneManager().update_current(engine, dt);
        window.clear();
        engine.getSceneManager().render_current(engine);
        window.display();
    }
    std::cout << "Offline game loop finished." << std::endl;
}

void ServerOfflineApplication::shutdown()
{
    std::cout << "Shutting down ServerOfflineApplication..." << std::endl;
    engine.shutdown();
    std::cout << "ServerOfflineApplication shutdown complete." << std::endl;
}