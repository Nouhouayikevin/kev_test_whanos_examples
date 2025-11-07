#include "../include/ServerApplication.hpp"
#include <iostream>

ServerApplication::ServerApplication()
    : work_(std::make_unique<boost::asio::io_context::work>(io_context_))
    , running_(false)
{
    tick_count_ = 0;
    std::cout << "ServerApplication created." << std::endl;
}

ServerApplication::~ServerApplication()
{
    // stop();
    std::cout << "ServerApplication destroyed." << std::endl;
}

void ServerApplication::initialize(uint16_t port)
{
    if (running_.exchange(true)) {
        std::cerr << "Server is already running" << std::endl;
        return;
    }
    try {
        gameEngine_.setNetwork(std::make_unique<UDPListener>(io_context_, port));
        io_thread_ = std::thread([this]() {
            try {
                io_context_.run();
            } catch (const std::exception& e) {
                std::cerr << "IO Context error: " << e.what() << std::endl;
                running_ = false;
            }
        });
        lastTick_ = std::chrono::high_resolution_clock::now();
        gameEngine_.getNetworkManager().start();
        // udp_listener.start();
        // udpListener_->start();
        run();

    } catch (const std::exception& e) {
        std::cerr << "Failed to start server: " << e.what() << std::endl;
        shutdown();
        throw;
    }
}

void ServerApplication::run()
{
    std::cout << "Server running..." << std::endl;
    while (running_) {
        try {
            auto now = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTick_);
            if (elapsed >= std::chrono::milliseconds(1000)) {
                lastTick_ = now;
                //processNetworkEvents();
                tick_count_++;
                std::cout << "Tick: " << tick_count_ << std::endl;
                            
                // broadcast to clients
            }
            //std::this_thread::sleep_for(std::chrono::milliseconds(16));
        } catch (const std::exception& e) {
            std::cerr << "Error in server loop: " << e.what() << std::endl;
        }
    }
}

// void ServerApplication::processNetworkEvents()
// {
    
//     //check for new clients
//     auto& udp_listener = static_cast<UDPListener&>(gameEngine_.getNetworkManager());
//     if (udp_listener.hasNewClient()) {
//         Client newClient = udp_listener.getClient();
//         std::cout << "New client connected: " << newClient.getId() << std::endl;
//         roomManager_.assignClientToRoom(newClient);
//     }
    
//     //process input from clients
//     if (udp_listener.hasNewInput()) {
//         Input input = udp_listener.getInput();
//         if (input.playerId != 0) {
//             std::cout << "New input received: " << input.playerId << std::endl;
//             roomManager_.assign_Input_to_Room(input);
//         }
//     }
// }

void ServerApplication::shutdown()
{
    auto& udp_listener = static_cast<UDPListener&>(gameEngine_.getNetworkManager());

    if (!running_.exchange(false)) {
        return;
    }
    
    work_.reset();
    if (udp_listener.isRunning()) {
        gameEngine_.getNetworkManager().stop();
    }
    io_context_.stop();
    if (io_thread_.joinable()) {
        io_thread_.join();
    }
    // udp_listener.reset();    
    std::cout << "Server stopped" << std::endl;
}
