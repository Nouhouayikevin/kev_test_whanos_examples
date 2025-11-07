/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** ResourceManager.cpp
*/

#include "../../Include/GameEngine_Include/managers/ResourceManager.hpp"
#include <stdexcept>

#include "../../Include/All/interfaces/IMusic.hpp"
#include "../../Include/All/interfaces/ITexture.hpp"
#include "../../Include/All/interfaces/ISound.hpp"
#include "../../Include/All/interfaces/IFont.hpp"
#include "../../Include/All/interfaces/ISprite.hpp"



ResourceManager::ResourceManager() = default;
ResourceManager::~ResourceManager() = default;

ResourceManager::ResourceManager(std::unique_ptr<IResourceFactory> factory) 
    : _factory(std::move(factory)) {}

void ResourceManager::setResourceFactory(std::unique_ptr<IResourceFactory> factory) {
    _factory = std::move(factory);
}


size_t ResourceManager::load_texture(const std::string& path) {

    if (!_factory) {
        throw std::runtime_error("ResourceManager: No resource factory has been set!");
    }
        
    auto it = _texture_path_to_id.find(path);
    if (it != _texture_path_to_id.end()) {
        return it->second;
    }

    auto texture = _factory->createTexture();
    if (!texture->loadFromFile(path)) {
        throw std::runtime_error("ResourceManager: Impossible de charger la texture: " + path);
    }

    size_t new_id = _loaded_textures.size();

    _loaded_textures.push_back(std::move(texture));

    _texture_path_to_id[path] = new_id;

    return new_id;
}

size_t ResourceManager::load_sound(const std::string& path) {

    if (!_factory) {
        throw std::runtime_error("ResourceManager: No resource factory has been set!");
    }

    auto it = _sound_path_to_id.find(path);
    if (it != _sound_path_to_id.end()) {
        return it->second;
    }

    auto sound = _factory->createSound();
    if (!sound->loadFromFile(path)) {
        throw std::runtime_error("ResourceManager: Impossible de charger le son: " + path);
    }
    
    size_t new_id = _loaded_sounds.size();
    _loaded_sounds.push_back(std::move(sound));
    _sound_path_to_id[path] = new_id;
    return new_id;

}

size_t ResourceManager::load_font(const std::string& path) {

    if (!_factory) {
        throw std::runtime_error("ResourceManager: No resource factory has been set!");
    }

    auto it = _font_path_to_id.find(path);
    if (it != _font_path_to_id.end()) {
        return it->second;
    }

    auto font = _factory->createFont();
    if (!font->loadFromFile(path)) {
        throw std::runtime_error("ResourceManager: Impossible de charger la police: " + path);
    }

    size_t new_id = _loaded_fonts.size();
    _loaded_fonts.push_back(std::move(font));
    _font_path_to_id[path] = new_id;
    return new_id;
}

size_t ResourceManager::load_music(const std::string& path) {
    if (!_factory) {
        throw std::runtime_error("ResourceManager: No resource factory has been set!");
    }

    auto it = _music_path_to_id.find(path);
    if (it != _music_path_to_id.end()) {
        return it->second;
    }

    auto music = _factory->createMusic();
    if (!music->openFromFile(path)) {
        throw std::runtime_error("ResourceManager: Impossible d'ouvrir la musique: " + path);
    }
    
    size_t new_id = _loaded_musics.size();
    _loaded_musics.push_back(std::move(music));
    _music_path_to_id[path] = new_id;
    return new_id;
}

ITexture& ResourceManager::get_texture(size_t id) {
    if (id >= _loaded_textures.size()) {
        throw std::out_of_range("ResourceManager: ID de texture "+ std::string("invalide."));
    }
    return *_loaded_textures[id];
}

ISound& ResourceManager::get_sound(size_t id) {
    if (id >= _loaded_sounds.size()) {
        throw std::out_of_range("ResourceManager: ID de son invalide.");
    }
    return *_loaded_sounds[id];
}

IFont& ResourceManager::get_font(size_t id) {
    if (id >= _loaded_fonts.size()) {
        throw std::out_of_range("ResourceManager: ID de police invalide.");
    }
    return *_loaded_fonts[id];
}

IMusic& ResourceManager::get_music(size_t id) {
    if (id >= _loaded_musics.size()) {
        throw std::out_of_range("ResourceManager: ID de musique invalide.");
    }
    return *_loaded_musics[id];
}

std::unique_ptr<ISprite> ResourceManager::createSprite() {
    if (!_factory) {
        throw std::runtime_error("ResourceManager: No resource factory has been set!");
    }
    return (_factory->createSprite());
}
