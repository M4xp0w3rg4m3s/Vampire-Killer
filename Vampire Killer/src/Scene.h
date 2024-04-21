#pragma once
#include <raylib.h>
#include "Player.h"
#include "TileMap.h"
#include "Object.h"
#include "AudioPlayer.h"

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
    AppStatus LoadLevel(int stage);
    
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
    const Texture2D* game_over;

    bool deathExecuted;
    bool renderingGameOver;

    Camera2D camera;
    DebugMode debug;
};

