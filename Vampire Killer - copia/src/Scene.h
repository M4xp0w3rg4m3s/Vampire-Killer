#pragma once
#include <raylib.h>
#include "Player.h"
#include "TileMap.h"
#include "Object.h"
#include "AudioPlayer.h"
#include "Text.h"

enum class DebugMode { OFF, SPRITES_AND_HITBOXES, ONLY_HITBOXES, SIZE };

class Scene
{
public:
    Scene();
    ~Scene();

    AppStatus Init();
    void Update();
    void Render();
    void Release();
    bool PlayerIsDead() const;
    bool PlayerHasWon() const;

private:
    AppStatus LoadLevel(int stage,int floor);
    
    void CheckCollisions(); 
    void ClearLevel();
    void RenderGameOver() const;
    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;

    void RenderGUI() const;

    Player *player;
    TileMap *level;
    std::vector<Object*> objects;
    int currentLevel;
    int currentFloor;
    const Texture2D* game_over;
    const Texture2D* hud;
    const Texture2D* chest_animation;
    int chest_time;

    bool deathExecuted;
    bool renderingGameOver;
    bool chestOpening;
    
    float currentChestX;
    float currentChestY;
    ObjectType currentChestType;

    Camera2D camera;
    DebugMode debug;

    Text* font;
};

