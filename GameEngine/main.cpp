/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** main.cpp
*/

#include "../Include/GameEngine_Include/testClass/TestApplication.hpp"
#include <iostream>
int main() {
    try {
        TestApplication app;

        app.initialize();
        app.run();
        app.shutdown();

    } catch (const std::exception& e) {
        std::cerr << "An unhandled exception occurred: " << e.what() << std::endl;
        return 84;
    } catch (...) {
        std::cerr << "An unknown exception occurred." << std::endl;
        return 84;
    }

    return 0;
}