#include "EnemyZombie.h"
#include "EnemyPanther.h"
#include "EnemyBat.h"
#include "EnemyManager.h"
#include "Trader.h"

EnemyManager::EnemyManager() {
	AudioPlayer::Instance().CreateSound("audio/SFX/27.wav", "Attack");
}

void EnemyManager::Update()
{
	for (int i = 0; i < enemies.size(); ++i) {
		if (enemies[i]->isActive) {
			enemies[i]->Update();
		}
	}
}
void EnemyManager::Reset()
{
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->Reset();
	}
}
void EnemyManager::SetTilemap(TileMap* tilemap)
{
	if (tilemap != nullptr) {
		map = tilemap;
		for (int i = 0; i < enemies.size(); i++) {
			enemies[i]->SetTileMap(map);
		}
	}
}
void EnemyManager::SetTarget(Player* player)
{
	target = player;
}
void EnemyManager::SpawnZombie(Point pos)
{
	Enemy* newEnemy = new EnemyZombie(pos);
	enemies.push_back(newEnemy);
}
void EnemyManager::SpawnPanther(Point pos)
{
	Enemy* newEnemy = new EnemyPanther(pos);
	enemies.push_back(newEnemy);
}
void EnemyManager::SpawnBat(Point pos)
{
	Enemy* newEnemy = new EnemyBat(pos);
	enemies.push_back(newEnemy);
}
void EnemyManager::SpawnTrader(Point pos)
{
	Enemy* newEnemy = new Trader(pos);
	enemies.push_back(newEnemy);
}
void EnemyManager::Render()
{
	for (int i = 0; i < enemies.size(); ++i) {
		if (enemies[i]->isActive) {
			enemies[i]->Render();
		}
	}
}
void EnemyManager::RenderDebug()
{
	for (int i = 0; i < enemies.size(); ++i) {
		if (enemies[i]->isActive) {
			enemies[i]->DrawDebug(YELLOW);
		}
	}
}
void EnemyManager::DestroyEnemies()
{
	for (size_t i = 0; i < enemies.size(); i++)
	{
		delete enemies[i];
	}
	enemies.clear();
}
EnemyManager::~EnemyManager()
{
	DestroyEnemies();
}
