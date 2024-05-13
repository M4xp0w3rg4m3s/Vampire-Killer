#pragma once
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include <unordered_map>
#include "raylib.h"
#include <vector>
using namespace std;

class EnemyManager : public Entity
{
public:
    static EnemyManager& Instance()
    {
        static EnemyManager instance; //Guaranteed to be initialized only once
        return instance;
    }
    void Update() override;

    void Reset();
    void SetTilemap(TileMap* tilemap);
    void SetTarget(Player* player);

    void SpawnZombie(Point pos);
    void SpawnPanther(Point pos);

    void Render();
    void RenderDebug();

    void DestroyEnemies();

    Player* target;
private:

    //Private constructor to prevent instantiation
    EnemyManager();
    //Destructor
    ~EnemyManager();

    vector<Enemy*> enemies;
    TileMap* map;

};