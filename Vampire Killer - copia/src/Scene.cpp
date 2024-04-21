#include "Scene.h"
#include <stdio.h>
#include "Globals.h"
#include "EnemyManager.h"

Scene::Scene()
{
	player = nullptr;
    level = nullptr;
	game_over = nullptr;
	currentLevel = 0;
	camera.target = { 0, 0 };				//Center of the screen
	camera.offset = { SIDE_MARGINS, TOP_MARGIN };	//Offset from the target (center of the screen)
	camera.rotation = 0.0f;					//No rotation
	camera.zoom = 1.0f;						//Default zoom
	deathExecuted = false;
	renderingGameOver = false;

	debug = DebugMode::OFF;
}
Scene::~Scene()
{
	if (player != nullptr)
	{
		player->Release();
		delete player;
		player = nullptr;
	}
    if (level != nullptr)
    {
		level->Release();
        delete level;
        level = nullptr;
    }
	for (Entity* obj : objects)
	{
		delete obj;
	}
	objects.clear();

}
AppStatus Scene::Init()
{
	ResourceManager& data = ResourceManager::Instance();

	//Create player
	player = new Player({ 20,144 }, State::IDLE, Look::RIGHT);
	EnemyManager::Instance().target = player;
	if (player == nullptr)
	{
		LOG("Failed to allocate memory for Player");
		return AppStatus::ERROR;
	}
	//Initialise player
	if (player->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Player");
		return AppStatus::ERROR;
	}

	//Create level 
    level = new TileMap();
    if (level == nullptr)
    {
        LOG("Failed to allocate memory for Level");
        return AppStatus::ERROR;
    }
	//Initialise level
	if (level->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	//Load level
	if (LoadLevel(1) != AppStatus::OK)
	{
		LOG("Failed to load Level 1");
		return AppStatus::ERROR;
	}
	//Assign the tile map reference to the player to check collisions while navigating
	player->SetTileMap(level);
	EnemyManager::Instance().SetTilemap(level);

	//Add the Game Over image for the end
	if (data.LoadTexture(Resource::IMG_GAME_OVER, "images/Spritesheets/HUD Spritesheet/GameOver.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	game_over = data.GetTexture(Resource::IMG_GAME_OVER);

	AudioPlayer::Instance().CreateMusic("audio/Music/02 Vampire Killer.ogg", "VampireKiller");
	AudioPlayer::Instance().SetMusicLoopStatus("VampireKiller",true);

	AudioPlayer::Instance().CreateSound("audio/SFX/1.wav", "Collect");

	player->weapon->SetWeapon(WeaponType::WHIP);

    return AppStatus::OK;
}
AppStatus Scene::LoadLevel(int stage)
{
	int size;
	int x, y, i;
	Tile tile;
	Point pos;
	int *mapBack = nullptr;
	int *map = nullptr;
	int *mapFront = nullptr;
	//Object *obj = nullptr;
	Object* obj;
	
	ClearLevel();

	size = LEVEL_WIDTH * LEVEL_HEIGHT;
	if (stage == 1)
	{
		currentLevel = 1;
		mapBack = new int[size] {
			  0,  1,  2,  3,  4,  1,  2,  3,  4,  1,  2,  3,  4,  1,  2,  3,  4,  0,
			  0,  5,  6,  7,  8,  5,  6,  7,  8,  5,  6,  7,  8,  5,  6,  7,  8,  0,
			  0,  9, 10, 11, 12,  9, 10, 11, 12,  9, 10, 11, 12,  9, 10, 11, 12,  0,
			  0, 13, 14, 15, 16, 13, 14, 15, 16, 13, 14, 15, 16, 13, 14, 15, 16,  0,
			  0, 17, 18, 19, 20, 17, 18, 19, 20, 17, 18, 19, 20, 17, 18, 19, 20,  0,
			  0, 21, 22, 23, 24, 25, 26, 27, 28, 21, 26, 27, 28, 25, 26, 27, 28,  0,
			  0, 29, 30, 41, 42, 31, 32, 29, 32, 29, 32, 29, 33, 34, 32, 29, 32,  0,
			  0, 36, 35, 43, 44, 35, 35, 36, 35, 36, 35, 36, 35, 35, 35, 36, 35,  0,
			  0, 37, 37, 45, 46, 38, 37, 37, 37, 37, 37, 37, 37, 38, 37, 37, 37,  0,
			  0, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,  0,
			  0, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,  0,
			};
		map = new int[size] {
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			550,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,501,
			550,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,501,
			550,  0,  0,  0,  0,124,  0,  0,  0,  0,  0,  0,  0,124,  0,  0,  0,501,
			550,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,501,
			550, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,501,
			  0, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,  0
		};
		mapFront = new int[size] {
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		};
		EnemyManager::Instance().DestroyEnemies();
		EnemyManager::Instance().SpawnZombie({ 236,143 });
		if (player->isGUIinit == false) {
			player->InitGUI();
		}
	}
	else if (stage == 2)
	{
		currentLevel = 2;
		mapBack = new int[size] {
			  0,  1,  2,  3,  4,  1,  2,  3,  4,  1,  2,  3,  4,  1,  2,  3,  4,  0,
			  0,  5,  6,  7,  8,  5,  6,  7,  8,  5,  6,  7,  8,  5,  6,  7,  8,  0,
			  0,  9, 10, 11, 12,  9, 10, 11, 12,  9, 10, 11, 12,  9, 10, 11, 12,  0,
			  0, 13, 14, 15, 16, 13, 14, 15, 16, 13, 14, 15, 16, 13, 14, 15, 16,  0,
			  0, 17, 18, 19, 20, 17, 18, 19, 20, 17, 18, 19, 20, 17, 18, 19, 20,  0,
			  0, 21, 22, 23, 24, 25, 26, 27, 28, 21, 26, 27, 28, 25, 26, 27, 28,  0,
			  0, 29, 30, 41, 42, 31, 32, 29, 32, 29, 32, 29, 33, 34, 32, 29, 32,  0,
			  0, 36, 35, 43, 44, 35, 35, 36, 35, 36, 35, 36, 35, 35, 35, 36, 35,  0,
			  0, 37, 37, 45, 46, 38, 37, 37, 37, 37, 37, 37, 37, 38, 37, 37, 37,  0,
			  0, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,  0,
			  0, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,  0
		};
		map = new int[size] {
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			 500, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,501,
			 500, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,501,
			 500, 0,  0,  0,  0,124,  0,  0,  0,  0,  0,  0,  0,124,  0,  0,  0,501,
			 500, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,501,
			 500,39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,501,
			  0, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,  0
		};
		mapFront = new int[size] {
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		};
		EnemyManager::Instance().DestroyEnemies();
		EnemyManager::Instance().SpawnPanther({ 40,143 });
	}
	else if (stage == 3)
	{
		currentLevel = 3;
		mapBack = new int[size] {
			  0,  1,  2,  3,  4,  1,  2,  3,  4,  1,  2, 47, 48, 49, 48, 49, 48,  0,
			  0,  5,  6,  7,  8,  5,  6,  7,  8,  5,  6, 50, 51, 52, 52, 53, 54,  0,
			  0,  9, 10, 11, 12,  9, 10, 11, 12,  9, 10, 55, 56, 57, 58, 53, 59,  0,
			  0, 13, 14, 15, 16, 13, 14, 15, 16, 13, 14, 50, 60, 61, 40, 62, 63,  0,
			  0, 17, 18, 19, 20, 17, 18, 19, 20, 17, 18, 55, 65, 66, 40, 67, 68,  0,
			  0, 21, 22, 23, 24, 25, 26, 27, 28, 21, 26, 50, 60, 61, 40, 62, 63,  0,
			  0, 29, 30, 41, 42, 31, 32, 29, 32, 29, 32, 55, 65, 66, 40, 67, 68,  0,
			  0, 36, 35, 43, 44, 35, 35, 36, 35, 36, 35, 69, 70, 71, 40, 72, 73,  0,
			  0, 37, 37, 45, 46, 38, 37, 37, 37, 37, 37, 69, 70, 71, 40, 72, 73,  0,
			  0, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,  0,
			  0, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,  0
		};
		map = new int[size] {
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			500,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,505,  0,
			500,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,505,  0,
			500,  0,  0,  0,  0,124,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,505,  0,
			500,  0,  0,  0,  0, 129,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,505,  0,
			500, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,  0,
			  0, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,  0
		};
		mapFront = new int[size] {
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 72, 73,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 72, 73,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		};
		EnemyManager::Instance().DestroyEnemies();
	}
	else if (stage == 4)
	{
		currentLevel = 4;
		mapBack = new int[size] {
			  0, 74, 75, 82, 83,110,110, 97, 98, 74, 75, 86, 87,110,110,101,102,  0,
			  0, 76, 77, 84, 85,110,110, 99,100, 76, 77, 91,110,110,110,110,110,  0,
			  0, 76, 77, 86, 87,110,110,101,102, 76, 77, 92,110,110,110,110,110,  0,
			  0, 76, 77, 88,110,103,103,110,110, 76, 77, 95, 96,103,103,110,110,  0,
			  0, 76, 77, 89,110,103,103,110,110, 76, 77, 97, 98,103,103,110,110,  0,
			  0, 76, 77, 90,110,103,103,110,110, 76, 77, 99,100,103,103,110,110,  0,
			  0, 76, 77, 90,110,103,103,110,110, 76, 77,101,102,103,103,110,110,  0,
			  0, 76, 77, 91,110,103,103,110,110, 76, 77,110,110,103,103,110,110,  0,
			  0, 76, 77, 92,110,103,103,110,110, 76, 77,110,110,103,103,110,110,  0,
			  0, 80, 81,110,110,110,110,110,110, 80, 81,110,110,110,110,110,110,  0,
			  0,108,109,108,109,108,109,108,109,108,109,108,109,108,109,108,109,  0
		};
		map = new int[size] {
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			550,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,550,
			550,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,550,
			550,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,550,
			550,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,550,
			550,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,550,
			550,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,550,
			  0,108,109,108,109,108,109,108,109,108,109,108,109,108,109,108,109,  0
		};
		mapFront = new int[size] {
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
			  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
		};
	}
	else
	{
		//Error level doesn't exist or incorrect level number
		LOG("Failed to load level, stage %d doesn't exist", stage);
		return AppStatus::ERROR;	
	}

	//Entities and objects
	i = 0;
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)mapBack[i];
			if (tile == Tile::EMPTY)
			{
				mapBack[i] = 0;
			}
			else if (tile == Tile::PLAYER)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				player->SetPos(pos);
				mapBack[i] = 0;
			}
			

			++i;
		}
	}
	i = 0;
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)map[i];
			if (tile == Tile::EMPTY)
			{
				map[i] = 0;
			}
			else if (tile == Tile::PLAYER)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				player->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::CHAIN)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::CHAIN);
				objects.push_back(obj);
				map[i] = 0;
			}
			++i;
		}
	}
	i = 0;
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)mapFront[i];
			if (tile == Tile::EMPTY)
			{
				mapFront[i] = 0;
			}
			else if (tile == Tile::PLAYER)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				player->SetPos(pos);
				mapFront[i] = 0;
			}
			++i;
		}
	}
	//Tile map
	level->Load(map, mapFront, mapBack,LEVEL_WIDTH, LEVEL_HEIGHT);
	delete[] map;
	delete[] mapFront;
	delete[] mapBack;
	return AppStatus::OK;
}
void Scene::Update()
{
	Point p1, p2;
	AABB box;
	Point left_position(16+3, player->GetPos().y);
	Point right_position(256-3, player->GetPos().y);

	EnemyManager::Instance().SetTilemap(level);

	//Switch between the different debug modes: off, on (sprites & hitboxes), on (hitboxes) 
	if (IsKeyPressed(KEY_F2))
	{
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);
	}
	//Debug levels instantly

	AudioPlayer::Instance().PlayMusicByName("VampireKiller");

	if (IsKeyPressed(KEY_ONE))			LoadLevel(1);
	else if (IsKeyPressed(KEY_TWO))		LoadLevel(2);
	else if (IsKeyPressed(KEY_THREE))	LoadLevel(3);
	else if (IsKeyPressed(KEY_C))		player->weapon->SetWeapon(WeaponType::CHAIN);
	else if (IsKeyPressed(KEY_F3)) {
		AudioPlayer::Instance().StopMusicByName("VampireKiller");
		player->Win();
	}
	else if (IsKeyPressed(KEY_F1))	    player->GodModeSwitch();
	else if (IsKeyPressed(KEY_F4))
	{
		if (!player->IsGodMode()) {
			player->StartDying();
		}
	}

	box = player->GetHitbox();

	AudioPlayer::Instance().Update();

	if (level->TestCollisionRight(box)) {
		if (currentLevel < 4) {
			LoadLevel(currentLevel + 1);
			player->SetPos(left_position);
		}
		else {
			LoadLevel(1);
		}
	}
	else if (level->TestCollisionLeft(box)) {
		if (currentLevel > 1) {
			LoadLevel(currentLevel - 1);
			player->SetPos(right_position);
		}
		else {
			LoadLevel(1);
		}
	}
	if (level->TestCollisionWin(box)) {
		AudioPlayer::Instance().StopMusicByName("VampireKiller");
		player->Win();
	}

	level->Update();
	EnemyManager::Instance().Update();
	player->Update();
	CheckCollisions();
}
void Scene::Render()
{
	BeginMode2D(camera);
	if (!player->IsDead()) {
		level->RenderEarly();

		level->Render();
		if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
		{
			RenderObjects(); 
			EnemyManager::Instance().Render();
			player->Draw();
			player->weapon->Draw();
		}
		if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
		{
			RenderObjectsDebug(YELLOW);
			EnemyManager::Instance().RenderDebug();
			player->DrawDebug(GREEN);
			player->weapon->DrawDebug(RED);

		}
		level->RenderLate();

		deathExecuted = false;
	}
	else if(player->IsDead() && deathExecuted == false){
		if (player->GetLives() <= 0) {
			AudioPlayer::Instance().StopMusicByName("VampireKiller");
			renderingGameOver = true;
			deathExecuted = true;
		}
		else {
			player->DecrLives(1);
			deathExecuted = true;
			AudioPlayer::Instance().StopMusicByName("VampireKiller");
			
			player->SetPos({ 20,140 });
			player->SetState(State::IDLE);
			player->weapon->SetWeapon(WeaponType::WHIP);
			player->SetLook(Look::RIGHT);

			LoadLevel(1);
		}
	}

	if (renderingGameOver) {
		RenderGameOver();
	}

	EndMode2D();

	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES) {
		if (player->IsGodMode()) {
			DrawText("GOD MODE : ON", 10, 30, 8, LIGHTGRAY);
		}
		else {
			DrawText("GOD MODE : OFF", 10, 30, 8, LIGHTGRAY);
		}
	}

	RenderGUI();
}
void Scene::Release()
{
    level->Release();
	player->Release();
	ClearLevel();
}
bool Scene::PlayerIsDead() const {
	if (player->GetLives() <= 0) {
		return player->IsDead();
	}
	else {
		return false;
	}
}
bool Scene::PlayerHasWon() const {
	return player->HasWon();
}
void Scene::CheckCollisions()
{
	AABB player_box, obj_box;
	
	player_box = player->GetHitbox();
	auto it = objects.begin();
	while (it != objects.end())
	{
		obj_box = (*it)->GetHitbox();
		if(player_box.TestAABB(obj_box))
		{
			AudioPlayer::Instance().PlaySoundByName("Collect");

			if ((*it)->GetType() == ObjectType::CHAIN) {

				player->weapon->SetWeapon(WeaponType::CHAIN);
			}
			//Delete the object
			delete* it; 
			//Erase the object from the vector and get the iterator to the next valid element
			it = objects.erase(it); 
		}
		else
		{
			//Move to the next object
			++it; 
		}
	}
}
void Scene::ClearLevel()
{
	for (Object* obj : objects)
	{
		delete obj;
	}
	objects.clear();
}
void Scene::RenderObjects() const
{
	for (Object* obj : objects)
	{
		obj->Draw();
	}
}
void Scene::RenderObjectsDebug(const Color& col) const
{
	for (Object* obj : objects)
	{
		obj->DrawDebug(col);
	}
}
void Scene::RenderGUI() const
{
	int currentLives = player->GetLives();
	DrawText(TextFormat("SCORE : %d", player->GetScore()), 10, 10, 8, LIGHTGRAY);
	if (currentLives >= 0) {
		DrawText(TextFormat("LIVES : %d", player->GetLives()), 10, 20, 8, LIGHTGRAY);
	}
	else {
		DrawText("LIVES : 0", 10, 20, 8, LIGHTGRAY);
	}
}
void Scene::RenderGameOver() const
{
	DrawTexture(*game_over, 16, 0, WHITE);
}