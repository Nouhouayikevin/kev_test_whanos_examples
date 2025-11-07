/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** main_client.cpp
*/


#include "../../Include/Client_include/ClientApplication.hpp"
#include <iostream>
#include <memory>
#include <cstring>

int main(int ac, char **av) {
    try {
        ClientApplication app; 

        std::string ip_arg;
        uint16_t port_arg = 0;
        for (int i = 1; i < ac; i++) {
            if ((std::strcmp(av[i], "-ip") == 0 || std::strcmp(av[i], "--ip") == 0) && i + 1 < ac) {
                ip_arg = av[++i];
            } else if ((std::strcmp(av[i], "-p") == 0 || std::strcmp(av[i], "--port") == 0) && i + 1 < ac) {
                int p = std::atoi(av[++i]);
                if (p > 0 && p <= 65535) port_arg = static_cast<uint16_t>(p);
            }
        }
        if (!ip_arg.empty() || port_arg != 0)
            SetDefaultServer(ip_arg, port_arg);

        app.initialize();

        app.run();

        app.shutdown();

    } catch (const std::exception& e) {
        std::cerr << "An unhandled exception occurred in ClientApplication: " << e.what() << std::endl;
        return 84;
    } catch (...) {
        std::cerr << "An unknown exception occurred in ClientApplication." << std::endl;
        return 84;
    }

    return 0;
}