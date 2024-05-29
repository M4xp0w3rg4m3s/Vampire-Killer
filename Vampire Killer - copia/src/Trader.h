#pragma once

#include "Enemy.h"
#include "EnemyManager.h"

#define TRADER_SPRITE_HEIGHT	32
#define TRADER_SPRITE_WIDTH		32

#define TRADER_HITBOX_HEIGHT	16
#define TRADER_HITBOX_WIDTH		32


class Trader : public Enemy
{
public:
	Trader(Point pos);
	~Trader();

	AppStatus Initialise();

	void Update();
	void Render();

	void Reset();
	void Brain();

	void SetTileMap(TileMap* tilemap);

	void DrawDebug(const Color& col) const;
	void Release();

	void SetAnimation(int id);

	bool PopUp;

	int AnimationFrame;

private:

	TileMap* map;
	EnemyAnim currentAnimation;
};



