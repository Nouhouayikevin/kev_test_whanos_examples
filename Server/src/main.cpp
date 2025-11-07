#include "../include/ServerApplication.hpp"
#include <csignal>
#include <atomic>
#include <iostream>

// static std::atomic<bool> g_running{true};
static ServerApplication* g_server = nullptr;
// static CentralServer* g_server = nullptr;

void handleSignal(int signum) {
    std::cout << "\nReceived signal " << signum << ", shutting down..." << std::endl;
    // g_running = false;
    if (g_server) {
        // g_server->stop();
        g_server->shutdown();
        std::exit(0);
    }
}

int main(int ac, char **av) {
    try {
        
        std::signal(SIGINT, handleSignal);
        std::signal(SIGTERM, handleSignal);
        
        if (ac != 2)
            throw Error("Invalid number of arguments");

        uint16_t port = std::stoi(av[1]);
    
        std::cout << "Starting R-Type server..." << std::endl;
        ServerApplication app;
        g_server = &app;
        app.initialize(port);

        // CentralServer server;
        // g_server = &server;
        // server.start(5000);        

        // while (g_running) {
        //     std::cout << "Runnning main..." << std::endl;
        //     std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // }
        std::cout << "Server shutdown complete." << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 84;
    } catch (const Error& er) {
        std::cerr << "Error: " << er.what() << std::endl;
        return 84;
    }
}
