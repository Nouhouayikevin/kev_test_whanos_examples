/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** GameApplication.hpp
*/

#pragma once

#include "../../All/interfaces/IApplication.hpp"
#include "../core/GameEngine.hpp"
#include "../../All/managers/SceneManager.hpp"

class TestApplication : public IApplication {
private:
    GameEngine _engine;
    SceneManager _scene_manager;

public:
    TestApplication();
    ~TestApplication();

    void initialize() override;
    void run() override;
    void shutdown() override;
};
