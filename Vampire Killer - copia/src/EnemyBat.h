#pragma once
#include "Enemy.h"

#define BAT_SPEED	2

#define BAT_SPRITE_HEIGHT	32
#define BAT_SPRITE_WIDTH	32

#define BAT_HITBOX_HEIGHT	16
#define BAT_HITBOX_WIDTH	32

#define BAT_DAMAGE			3

class EnemyBat : public Enemy
{
public:

	struct Wave {
		float y0;
		float amplitude;
		float alpha;
		float delta;
	};
	EnemyBat(Point pos);
	virtual ~EnemyBat();

	AppStatus Initialise();

	void Update() override;
	void Render() override;

	void Reset() override;
	void Brain() override;

	void SetTileMap(TileMap* tilemap) override;

	void AdvanceRight();
	void AdvanceLeft();
	void Move();

	void DrawDebug(const Color& col) const override;
	void Release() override;

	void SetAnimation(int id);

private:

	TileMap* map;

};

