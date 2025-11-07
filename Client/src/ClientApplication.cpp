/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Oct, 2025                                                     *
 * Title           - G-CPP-500-COT-5-1-rtype-24                                         *
 * Description     -                                                                    *
 *     ClientApplication                                                                *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *             ███████╗██████╗ ██╗████████╗███████╗ ██████╗██╗  ██╗                     *
 *             ██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝██║  ██║                     *
 *             █████╗  ██████╔╝██║   ██║   █████╗  ██║     ███████║                     *
 *             ██╔══╝  ██╔═══╝ ██║   ██║   ██╔══╝  ██║     ██╔══██║                     *
 *             ███████╗██║     ██║   ██║   ███████╗╚██████╗██║  ██║                     *
 *             ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝                     *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */



#include "../../Include/Client_include/ClientApplication.hpp"
#include "../include/OfflineApplication.hpp"
#include "../include/OnlineApplication.hpp"
#include "../All/sfml/SfmlWindow.hpp"
#include "../All/sfml/SfmlInputManager.hpp"
#include "../All/sfml/SfmlResourceFactory.hpp"
#include "../All/Scenes/MenuScene.hpp"
#include <iostream>

static ClientApplication* g_client_app = nullptr;
static std::string g_default_ip = "127.0.0.1";
static uint16_t g_default_port = 4242;

ClientApplication::ClientApplication()
{
    engine = std::make_shared<GameEngine>();
    std::cout << "ClientApplication created." << std::endl;
    g_client_app = this;
}

ClientApplication::~ClientApplication()
{
    if (current_app)
        current_app->shutdown();
    g_client_app = nullptr;
    std::cout << "ClientApplication destroyed." << std::endl;
}

void ClientApplication::initialize()
{
    std::cout << "Initializing ClientApplication..." << std::endl;
    auto window = std::make_unique<SfmlWindow>(1280, 720, "R-Type");
    auto input = std::make_unique<SfmlInputManager>(*window);
    auto resource_factory = std::make_unique<SfmlResourceFactory>();

    engine->setWindow(std::move(window));
    engine->setInputManager(std::move(input));
    engine->getResourceManager().setResourceFactory(std::move(resource_factory));

    engine->getSceneManager().add_scene("menu", std::make_shared<MenuScene>());
    engine->getSceneManager().switch_to_scene("menu", *engine);

    engine->initialize();
    std::cout << "ClientApplication initialized successfully." << std::endl;
}

void ClientApplication::setMode(const std::string& mode, const std::string& ip, uint16_t port)
{
    if (current_app && current_mode == mode) {
        return;
    }
    if (mode == "offline") {
        auto offline_app = std::make_unique<OfflineApplication>();
        offline_app->setEngine(engine);
        current_app = std::move(offline_app);
    } else if (mode == "online") {
        const std::string use_ip = ip.empty() ? g_default_ip : ip;
        const uint16_t use_port = (port == 0) ? g_default_port : port;
        auto online_app = std::make_unique<OnlineApplication>(use_ip, use_port);
        online_app->setEngine(engine);
        current_app = std::move(online_app);
    }
    current_mode = mode;
    if (current_app)
        current_app->initialize();
}

void ClientApplication::run()
{
    std::cout << "Starting ClientApplication game loop..." << std::endl;
    if (!current_app) {
        IWindow& window = engine->getWindowManager();
        IInputManager& input = engine->getInputManager();
        Clock& clock = engine->getClock();

        while (!current_app && window.isOpen()) {
            float dt = clock.restart();
            input.update();
            if (input.isWindowClosed())
                window.close();
            engine->getSceneManager().update_current(*engine, dt);
            window.clear();
            engine->getSceneManager().render_current(*engine);
            window.display();
        }
    }
    if (current_app)
        current_app->run();
    std::cout << "ClientApplication game loop finished." << std::endl;
}

void ClientApplication::shutdown()
{
    std::cout << "Shutting down ClientApplication..." << std::endl;
    if (current_app)
        current_app->shutdown();
    engine->shutdown();
    std::cout << "ClientApplication shutdown complete." << std::endl;
}

void SetMode(const std::string& mode, const std::string& ip, uint16_t port)
{
    if (!g_client_app)
        return;
    g_client_app->setMode(mode, ip, port);
}

void SetDefaultServer(const std::string& ip, uint16_t port)
{
    if (!ip.empty())
        g_default_ip = ip;
    if (port != 0)
        g_default_port = port;
}