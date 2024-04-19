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
    AudioPlayer* audio;
    std::vector<Object*> objects;
    int currentLevel;
    const Texture2D* game_over;

    Camera2D camera;
    DebugMode debug;
};

