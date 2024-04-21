//#pragma once
//#include "Entity.h"
//#include "Weapon.h"
//#include "TileMap.h"
//#include "Player.h"
//#include "EnemyZombie.h"
//#include "EnemyManager.h"
//
////Logic states
//enum class EnemyState {
//	IDLE, ADVANCING, DEAD
//};
//enum class EnemyLook { RIGHT, LEFT };
//
////Rendering states
//enum class EnemyAnim {
//	IDLE_LEFT, IDLE_RIGHT,
//	ADVANCING_LEFT, ADVANCING_RIGHT,
//	NUM_ANIMATIONS
//};
//
//class Enemy : public Entity
//{
//public:
//	Enemy();
//	virtual ~Enemy();
//
//	AppStatus Initialise();
//
//	void Update();
//	void Render();
//
//	void SetType(EnemyType type);
//	EnemyType GetType() const;
//
//	void Spawn();
//	void Spawn(const Point& position);
//
//	void DrawDebug(const Color& col) const;
//	void Release();
//
//protected:
//
//	Enemy* thisEnemy;
//	EnemyType thisType;
//
//	EnemyState state;
//	EnemyLook look;
//	TileMap* map;
//	Player* player;
//
//	int AnimationFrame;
//	int EnemySpeed;
//
//	int EnemyFrameHeight;
//	int EnemyFrameWidth;
//
//	int EnemyHitboxHeight;
//	int EnemyHitboxWidth;
//
//
//};
//
