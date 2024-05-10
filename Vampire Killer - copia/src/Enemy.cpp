
#include "Enemy.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include "Weapon.h"
#include <raymath.h>
#include "EnemyManager.h"

Enemy::Enemy(Point pos, int height, int width, int frameheight, int framewidth) : Entity(pos, width, height, framewidth, frameheight)
{
	Damage = 0;
}
Enemy::~Enemy()
{
}
AppStatus Enemy::Initialise()
{
	return AppStatus::OK;
}
void Enemy::Update()
{
}
void Enemy::Render()
{
}
void Enemy::Reset()
{
}
void Enemy::Brain()
{
}
void Enemy::SetTileMap(TileMap* tilemap)
{
}
void Enemy::DrawDebug(const Color& col) const
{
}
void Enemy::Release()
{
	render->Release();
}
void Enemy::DamagePlayer() const
{
	EnemyManager::Instance().target->DecrLife(Damage);
}


