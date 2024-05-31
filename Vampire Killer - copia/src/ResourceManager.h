#pragma once

#include "raylib.h"
#include "Globals.h"
#include <unordered_map>
#include <string>

enum class Resource {
    IMG_MENU,
    IMG_INTRO_UPC,
    IMG_INTRO_BACKGROUND,
    IMG_INTRODUCTION,
    IMG_INTRO_BAT,
    IMG_INTRO_CLOUD,
    IMG_MENU_PLAY,
    IMG_MENU_EMPTY,
    IMG_PLAYER, 
    IMG_ZOMBIE,
    IMG_PANTHER,
    IMG_HIT_EFFECT,
    IMG_WEAPON,
    IMG_TILES,
    IMG_GAME_OVER,
    IMG_GAME_WIN,
    IMG_FONT,
    IMG_OPEN_CHEST,
    IMG_HUD,
    IMG_HUD_INTRO,
    IMG_HUD_ITEMS,
    IMG_TRADER,
    IMG_BAT
};

class ResourceManager {
public:
    //Singleton instance retrieval
    static ResourceManager& Instance()
    {
        static ResourceManager instance; //Guaranteed to be initialized only once
        return instance;
    }

    //Load and unload texture
    AppStatus LoadTexture(Resource id, const std::string& file_path);
    void ReleaseTexture(Resource id);

    //Get texture by key
    const Texture2D* GetTexture(Resource id) const;

    //Release resources
    void Release();

    //Ensure Singleton cannot be copied or assigned
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

private:
    //Private constructor to prevent instantiation
    ResourceManager();
    //Destructor
    ~ResourceManager();

    //Dictionary to store loaded textures
    std::unordered_map<Resource, Texture2D> textures;
};