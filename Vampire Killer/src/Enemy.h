#pragma once
#include "Entity.h"
#include "Weapon.h"
#include "TileMap.h"

//Representation model size: 32x32
#define ENEMY_FRAME_SIZE	32

//Logical model size: 12x28
#define ENEMY_PHYSICAL_WIDTH	12
#define ENEMY_PHYSICAL_HEIGHT	28

//Horizontal speed and vertical speed while falling down
#define ENEMY_SPEED			1

//Logic states
enum class EnemyState {
	IDLE, WALKING, JUMPING, FALLING,
	DYING, DEAD
};
enum class EnemyLook { RIGHT, LEFT };

//Rendering states
enum class EnemyAnim {
	IDLE_LEFT, IDLE_RIGHT,
	WALKING_LEFT, WALKING_RIGHT,
	DYING,
	NUM_ANIMATIONS
};

// Types of enemies
enum class EnemyType {
	ZOMBIE, PANTHER
};

class Enemy : public Entity
{
public:
	Enemy(const Point& p, EnemyState s, EnemyLook view);
	~Enemy();

	AppStatus Initialise();

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

private:

	void SpawnEnemy(Point pos, EnemyType type);

	void SpawnZombieRight();
	void SpawnZombieLeft();

	void SpawnPantherRight();
	void SpawnPantherLeft();

	EnemyState state;
	EnemyLook look;
	int AnimationFrame;
};

