#pragma once
#include "Enemy.h"
#include "math.h"

#define BOSS_SPEED	1

#define BOSS_SPRITE_HEIGHT	32
#define BOSS_SPRITE_WIDTH	64

#define BOSS_HITBOX_HEIGHT	32
#define BOSS_HITBOX_WIDTH	32

#define BOSS_DAMAGE			6

class Boss : public Enemy
{
public:

	Boss(Point pos);
	virtual ~Boss();

	AppStatus Initialise();

	void Update() override;
	void Render() override;

	void Reset() override;
	void Brain() override;

	void SetTileMap(TileMap* tilemap) override;

	int GetLife() const override;

	void Move();

	void DrawDebug(const Color& col) const override;
	void Release() override;

private:

	int life = 16;

	bool justHit = false;
	int hitCounter = 30;

	int counter;
	int Straight_or_curve;
	int Curve_Up_or_Down;

	bool Moving = false;

	bool ToCalculateVec = true;
	Vector2 vec;

	TileMap* map;
	int Init_pos_y;

	float internalTimer = 0;
	float amplitude = 2.5;
	float currentAmplitude = 0;
	float amplitudeChangeSpeed = 3;
};

