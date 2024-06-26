#include "EnemyZombie.h"
#include "EnemyPanther.h"
#include "EnemyBat.h"
#include "EnemyManager.h"
#include "Trader.h"
#include "Boss.h"

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
void EnemyManager::SpawnBoss(Point pos)
{
	Enemy* newEnemy = new Boss(pos);
	enemies.push_back(newEnemy);
}
bool EnemyManager::GetTraderPopUp()
{
	for (int i = 0; i < enemies.size(); ++i) {
		if (enemies[i]->GetType() == EnemyType::TRADER) {
			return enemies[i]->GetPopUp();
		}
	}
}
bool EnemyManager::DeleteTraderPopUp()
{
	for (int i = 0; i < enemies.size(); ++i) {
		if (enemies[i]->GetType() == EnemyType::TRADER) {
			return enemies[i]->DeletePopUp();
		}
	}
}
bool EnemyManager::GetKilled()
{
	for (int i = 0; i < enemies.size(); ++i) {
		if (enemies[i]->isActive) {
			return enemies[i]->IsKilled();
			delete enemies[i];
			enemies[i]->Update();
		}
	}
}
Vector2 EnemyManager::GetKilledPos()
{
	for (int i = 0; i < enemies.size(); ++i) {
		if (enemies[i]->IsKilled()) {
			return { (float)target->weapon->HitboxOnAttack().pos.x, (float)target->weapon->HitboxOnAttack().pos.y };
		}
	}
}
bool EnemyManager::IsBossDead()
{
	for (int i = 0; i < enemies.size(); ++i) {
		if (enemies[i]->GetType() == EnemyType::BOSS) {
			if (enemies[i]->GetLife() == 0) {
				return true;
			}
			else {
				return false;
			}
		}
	}
}
int EnemyManager::GetBossLife()
{
	for (int i = 0; i < enemies.size(); ++i) {
		if (enemies[i]->GetType() == EnemyType::BOSS) {
			return enemies[i]->GetLife();
		}
	}
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
