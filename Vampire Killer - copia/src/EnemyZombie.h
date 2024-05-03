#pragma once
#include "Enemy.h"

#define ZOMBIE_SPEED	1

#define ZOMBIE_SPRITE_HEIGHT	32
#define ZOMBIE_SPRITE_WIDTH		32

#define ZOMBIE_HITBOX_HEIGHT	32
#define ZOMBIE_HITBOX_WIDTH		16

#define ZOMBIE_DAMAGE			1

class EnemyZombie : public Enemy
{
public:
	EnemyZombie(Point pos);
	virtual ~EnemyZombie();

	AppStatus Initialise();

	void Update() override;
	void Render() override;

	void Reset() override;
	void Brain() override;

	void SetTileMap(TileMap* tilemap) override;

	void AdvanceRight();
	void AdvanceLeft();
	void MoveX();

	void DrawDebug(const Color& col) const override;
	void Release() override;

	void SetAnimation(int id);

private:

	TileMap* map;

};

