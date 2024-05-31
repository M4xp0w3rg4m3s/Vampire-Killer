
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
EnemyType Enemy::GetType() const
{
	return type;
}
bool Enemy::GetPopUp() const
{
	return false;
}
bool Enemy::DeletePopUp() const
{
	return true;
}
void Enemy::Stop()
{
	dir = { 0,0 };
	state = EnemyState::IDLE;
	if (look == EnemyLook::RIGHT) SetAnimation((int)EnemyAnim::IDLE_RIGHT);
	else SetAnimation((int)EnemyAnim::IDLE_LEFT);
}
void Enemy::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
void Enemy::DamagePlayer() const
{
	EnemyManager::Instance().target->DecrLife(Damage);
}


