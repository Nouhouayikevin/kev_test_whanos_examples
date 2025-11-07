/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** SfmlClock.cpp
*/

#include "../../Include/GameEngine_Include/core/Clock.hpp"

Clock::Clock() {
    restart();
}

float Clock::getElapsedTime() const {
    auto now = std::chrono::steady_clock::now();
    
    std::chrono::duration<float> elapsed_seconds = now - _start_point;
    
    return elapsed_seconds.count();
}

float Clock::restart() {
    const auto now = std::chrono::steady_clock::now();
    
    std::chrono::duration<float> elapsed_seconds = now - _start_point;
    
    _start_point = now;
    
    return elapsed_seconds.count();
}