
#include "Enemy.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include "Weapon.h"
#include <raymath.h>

Enemy::Enemy()
{
}
Enemy::~Enemy()
{
}
AppStatus Enemy::Initialise()
{
	int i;
	AnimationFrame = 0;
	return AppStatus::OK;
}
void Enemy::Update()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void Enemy::Render()
{
}
void Enemy::SetSpawn(int id)
{
	if (id == 0) {
		if (player->GetPos().x > WINDOW_WIDTH) {
			SetPos({ 255, 176 });
		}
		else if (player->GetPos().x > WINDOW_WIDTH) {
			SetPos({ 20, 176 });
		}
	}
	else if (id == 1) {
		if (player->GetPos().x > WINDOW_WIDTH) {
			SetPos({ 255, 176 });
		}
		else if (player->GetPos().x > WINDOW_WIDTH) {
			SetPos({ 20, 176 });
		}
	}
}
void Enemy::DrawDebug(const Color& col) const
{
}
void Enemy::Release()
{
	player->Release();
	map->Release();
	render->Release();
}


