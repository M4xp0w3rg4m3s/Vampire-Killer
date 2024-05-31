#pragma once
#include "Enemy.h"
#include "math.h"

#define BAT_SPEED	1

#define BAT_SPRITE_HEIGHT	32
#define BAT_SPRITE_WIDTH	32

#define BAT_HITBOX_HEIGHT	16
#define BAT_HITBOX_WIDTH	16

#define BAT_DAMAGE			1

class EnemyBat : public Enemy
{
public:

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

private:

	TileMap* map;
	int Init_pos_y;

	float internalTimer = 0;
	float amplitude = 0.75;
	float currentAmplitude = 0;
	float amplitudeChangeSpeed = 3;

};

