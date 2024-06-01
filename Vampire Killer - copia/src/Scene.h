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
    void DeletePopUp();
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
    const Texture2D* loot_heart;
    const Texture2D* hud_items;
    const Texture2D* popup_trader;
    const Texture2D* hit_effect;
    int zombie_delay_time;
    int bat_delay_time;
    
    bool boss_spawned = false;

    int hit_effect_time = 30;
    bool enemy_killed = false;
    bool got_enemy_pos = false;
    Vector2 enemy_killed_pos;

    bool gotBoots = false;
    int boot_time = 60;
    bool gotWings = false;
    int wings_time = 60;
    bool gotShield = false;
    int shield_time = 60;
    bool gotHeart = false;
    int heart_time = 60;

    bool deathExecuted;
    bool renderingGameOver;

    //broken walls booleans
    bool level6WallBroken;
    bool level7_1WallBroken;
    
    //chest controllers
    int chest_time;
    bool chestOpening;
    float currentChestX;
    float currentChestY;
    ObjectType currentChestType;

    //fire / candle controllers
    int loot_time;
    bool lootOpening;
    float currentLootX;
    float currentLootY;
    ObjectType currentLootType;
    float spawnY;

    Camera2D camera;
    DebugMode debug;

    //Trader
    bool traderSpawned = false;
    bool popUpDisplayed = false;

    Text* font;
};

