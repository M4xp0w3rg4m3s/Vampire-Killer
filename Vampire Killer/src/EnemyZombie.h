#pragma once
#include "Enemy.h"

#define ZOMBIE_SPEED	1

#define ZOMBIE_SPRITE_HEIGHT	32
#define ZOMBIE_SPRITE_WIDTH		32

#define ZOMBIE_HITBOX_HEIGHT	32
#define ZOMBIE_HITBOX_WIDTH		32


class EnemyZombie : public Enemy
{
public:
	EnemyZombie();
	virtual ~EnemyZombie();

	AppStatus Initialise();

	void Update();
	void Render();

	void AdvanceRight();
	void AdvanceLeft();

	void MoveX();

	void Spawn();

	void DrawDebug(const Color& col) const;
	void Release();

	void SetAnimation(int id);
};

