#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <thread>
#include <atomic>
#include <chrono>
#include "UDPListener.hpp"
#include "RoomManager.hpp"
#include "Error.hpp"
#include "../../Include/GameEngine_Include/core/GameEngine.hpp"

class ServerApplication {
public:
    ServerApplication();
    ~ServerApplication();

    void shutdown();
    void run();
    void initialize(uint16_t port = 50000);
    // void start(uint16_t port = 50000);
    // void stop(); 

private:
////

///
    // void processNetworkEvents();

    GameEngine gameEngine_;
    boost::asio::io_context io_context_;
    std::unique_ptr<boost::asio::io_context::work> work_;
    std::unique_ptr<INetwork> udpListener_;
    // RoomManager roomManager_;
    std::atomic<bool> running_{false};
    std::thread io_thread_;
    //game tick counter
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTick_;
    int tick_count_;
};
