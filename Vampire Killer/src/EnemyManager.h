#pragma once
#include "Entity.h"
#include "Enemy.h"
#include <unordered_map>
#include "raylib.h"

enum class EnemyType {
    ZOMBIE, PANTHER
};

class EnemyManager : public Entity
{
public:
    static EnemyManager& Instance()
    {
        static EnemyManager instance; //Guaranteed to be initialized only once
        return instance;
    }
    void Update() override;

    void SetEnemy(int id, EnemyType type);
    void SpawnEnemy(EnemyType type);
    void SpawnEnemy(EnemyType type, Point pos);

    void Render();

private:

    //Private constructor to prevent instantiation
    EnemyManager();
    //Destructor
    ~EnemyManager();

    Enemy* enemies = new Enemy[10];

};