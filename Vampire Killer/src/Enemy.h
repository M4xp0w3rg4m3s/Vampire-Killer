#pragma once
#include "Entity.h"
#include "Weapon.h"
#include "TileMap.h"
#include "Player.h"

//Logic states
enum class EnemyState {
	IDLE, ADVANCING, DEAD
};
enum class EnemyLook { RIGHT, LEFT };

//Rendering states
enum class EnemyAnim {
	IDLE_LEFT, IDLE_RIGHT,
	ADVANCING_LEFT, ADVANCING_RIGHT,
	NUM_ANIMATIONS
};

class Enemy : public Entity
{
public:
	Enemy();
	virtual ~Enemy();

	AppStatus Initialise();

	void Update();
	void Render();

	void DrawDebug(const Color& col) const;
	void Release();

	void SetSpawn(int id);

protected:

	EnemyState state;
	EnemyLook look;
	TileMap* map;
	Player* player;

	int AnimationFrame;
	int EnemySpeed;

	int EnemyFrameHeight;
	int EnemyFrameWidth;

	int EnemyHitboxHeight;
	int EnemyHitboxWidth;


};

