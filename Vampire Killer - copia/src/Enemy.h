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
	EMPTY,
	NUM_ANIMATIONS
};

class Enemy : public Entity
{
public:
	Enemy(Point pos, int height, int width, int frameheight, int framewidth);
	virtual ~Enemy();

	virtual AppStatus Initialise();

	virtual void Update();
	virtual void Render();

	virtual void Reset();
	virtual void Brain();

	virtual void SetTileMap(TileMap* tilemap);

	virtual void DrawDebug(const Color& col) const;
	virtual void Release();

protected:

	bool isActive;

	EnemyState state;
	EnemyLook look;

	int AnimationFrame;
	int EnemySpeed;
};

