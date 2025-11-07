##
## EPITECH PROJECT, 2025
## G-CPP-500-COT-5-1-rtype-24
## File description:
## kev_test.sh
##

#!/bin/bash

echo "=== Compilation du TestApplication avec MenuScene ==="

g++ -std=c++20 -g  \
    GameEngine/main.cpp \
    GameEngine/TestApplication.cpp \
    GameEngine/Core/GameEngine.cpp \
    GameEngine/Core/Clock.cpp \
    GameEngine/Core/LuaEntity.cpp \
    GameEngine/managers/LevelManager.cpp \
    GameEngine/managers/ResourceManager.cpp \
    GameEngine/managers/ReplayManager.cpp \
    GameEngine/managers/ScriptingManager.cpp \
    All_src/managers/SceneManager.cpp \
    All_src/scenes/TestGameScene.cpp \
    All_src/scenes/MenuScene.cpp \
    All_src/sfml/SfmlWindow.cpp \
    All_src/sfml/SfmlInputManager.cpp \
    All_src/sfml/SfmlResourceFactory.cpp \
    All_src/sfml/SfmlSprite.cpp \
    All_src/sfml/SfmlTexture.cpp \
    All_src/sfml/SfmlFont.cpp \
    All_src/sfml/SfmlSound.cpp \
    All_src/sfml/SfmlMusic.cpp \
    -I./Include \
    -I./Include/All \
    -I/usr/include/lua5.3 \
    -I./Include/GameEngine_Include \
    -IInclude -I./Include/GameEngine_Include/core/sol2/include \
    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio \
    -llua5.3 \
    -o YourExecutableName

if [ $? -eq 0 ]; then
    echo "✅ Compilation réussie !"
    echo "=== Lancement de l'application ==="
    ./YourExecutableName
else
    echo "❌ Erreur de compilation"
    exit 1
fi