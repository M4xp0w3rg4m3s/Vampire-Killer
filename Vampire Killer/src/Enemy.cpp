
#include "Enemy.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include "Weapon.h"
#include <raymath.h>

Enemy::Enemy(const Point& p, EnemyState s, EnemyLook view) :
	Entity(p, ENEMY_PHYSICAL_WIDTH, ENEMY_PHYSICAL_HEIGHT, ENEMY_FRAME_SIZE, ENEMY_FRAME_SIZE)
{
	state = s;
	look = view;
	AnimationFrame = 0;
}
Enemy::~Enemy()
{
}
void Enemy::Update()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
AppStatus Enemy::Initialise()
{
	int i;
	const int n = ENEMY_FRAME_SIZE;
	AnimationFrame = 0;

	return AppStatus::OK;
}

void Enemy::SpawnEnemy(Point pos, EnemyType type) {

}

void Enemy::SpawnZombieRight() {

}
void Enemy::SpawnZombieLeft() {

}

void Enemy::SpawnPantherRight() {

}
void Enemy::SpawnPantherLeft() {

}

void Enemy::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);

	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 170, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}
void Enemy::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PLAYER);

	render->Release();
}