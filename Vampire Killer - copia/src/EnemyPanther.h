#pragma once
#include "Enemy.h"

#define PANTHER_SPEED	2

#define PANTHER_SPRITE_HEIGHT	32
#define PANTHER_SPRITE_WIDTH	32

#define PANTHER_HITBOX_HEIGHT	16
#define PANTHER_HITBOX_WIDTH	32

#define PANTHER_DAMAGE			3

class EnemyPanther : public Enemy
{
public:
	EnemyPanther(Point pos);
	virtual ~EnemyPanther();

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

private:

	TileMap* map;

};

