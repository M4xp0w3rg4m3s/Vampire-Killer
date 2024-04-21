
#include "EnemyManager.h"

EnemyManager::EnemyManager() {
}

void EnemyManager::Update()
{
	for (int i = 0; i < 10; ++i) {
		enemies[i].Update();
	}
}
void EnemyManager::SetEnemy(int id, EnemyType type)
{
	enemies[id].SetType(type);
}
void EnemyManager::SpawnEnemy(EnemyType type)
{
	for (int i = 0; i < 10; ++i) {
		if (enemies[i].GetType() == type) {
			enemies[i].Spawn();
			break;
		}
	}
}
void EnemyManager::SpawnEnemy(EnemyType type, Point pos)
{
	for (int i = 0; i < 10; ++i) {
		if (enemies[i].GetType() == type) {
			enemies[i].Spawn(pos);
			break;
		}
	}
}
void EnemyManager::Render()
{
	for (int i = 0; i < 10; ++i) {
		enemies[i].Render();
	}
}

EnemyManager::~EnemyManager()
{
	enemies->Release();
}
