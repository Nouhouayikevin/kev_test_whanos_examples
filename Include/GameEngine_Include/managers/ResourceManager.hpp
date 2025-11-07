/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** ResourceManager.hpp
*/


#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "../../All/interfaces/IResourceFactory.hpp"

class ITexture;
class ISound;
class IMusic;
class IFont;
class ISound;
class ISprite;


class ResourceManager {
public:
    ResourceManager();
    ResourceManager(std::unique_ptr<IResourceFactory> factory);
    ~ResourceManager();

    void setResourceFactory(std::unique_ptr<IResourceFactory> factory);

    size_t load_texture(const std::string& path);
    
    size_t load_sound(const std::string& path);
    size_t load_music(const std::string& path);
    
    size_t load_font(const std::string& path);

    ITexture& get_texture(size_t id);
    ISound& get_sound(size_t id);
    IFont& get_font(size_t id);
    IMusic& get_music(size_t id);

    std::unique_ptr<ISprite> createSprite();

private:
    std::unique_ptr<IResourceFactory> _factory;

    std::vector<std::unique_ptr<ITexture>> _loaded_textures;
    std::unordered_map<std::string, size_t> _texture_path_to_id;

    std::vector<std::unique_ptr<ISound>> _loaded_sounds;
    std::unordered_map<std::string, size_t> _sound_path_to_id;

    std::vector<std::unique_ptr<IFont>> _loaded_fonts;
    std::unordered_map<std::string, size_t> _font_path_to_id;

    std::vector<std::unique_ptr<IMusic>> _loaded_musics;
    std::unordered_map<std::string, size_t> _music_path_to_id;
};


