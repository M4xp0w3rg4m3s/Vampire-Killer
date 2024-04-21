#include "EnemyZombie.h"
#include "EnemyPanther.h"
#include "EnemyManager.h"

EnemyManager::EnemyManager() {
}

void EnemyManager::Update()
{
	for (int i = 0; i < enemies.size(); ++i) {
		enemies[i]->Update();
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
void EnemyManager::Render()
{
	for (int i = 0; i < enemies.size(); ++i) {
		enemies[i]->Render();
	}
}
void EnemyManager::RenderDebug()
{
	for (int i = 0; i < enemies.size(); ++i) {
		enemies[i]->DrawDebug(YELLOW);
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
