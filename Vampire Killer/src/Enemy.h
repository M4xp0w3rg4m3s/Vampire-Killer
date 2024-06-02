#pragma once
#include "Entity.h"
#include "Weapon.h"
#include "TileMap.h"
#include "Player.h"

//Logic states
enum class EnemyState {
	IDLE, ADVANCING, DEAD, FALLING
};
enum class EnemyLook { RIGHT, LEFT };

enum class EnemyType {
	ZOMBIE, PANTHER, BAT, TRADER, BOSS
};

//Rendering states
enum class EnemyAnim {
	IDLE_LEFT, IDLE_RIGHT,
	ADVANCING_LEFT, ADVANCING_RIGHT, 
	RED_TRADER,
	MOVING,
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

	bool IsKilled() const;

	Vector2 GetKilledPosition() const;

	virtual int GetLife() const;

	virtual EnemyType GetType() const;
	virtual bool GetPopUp() const;
	virtual bool DeletePopUp() const;

	bool isActive = true;

protected:

	void Stop();
	void SetAnimation(int id);

	void DamagePlayer() const;

	EnemyState state;
	EnemyLook look;
	EnemyType type;

	int AnimationFrame;
	int EnemySpeed;
	int Damage;

	bool killed = false;
};

