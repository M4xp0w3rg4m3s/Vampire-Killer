
#include "EnemyManager.h"

EnemyManager::EnemyManager() {
}

void EnemyManager::Update()
{
	for (int i = 0; i < 10; ++i) {
		enemies[i].Update();
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
