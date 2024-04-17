#include "Scene.h"
#include <stdio.h>
#include "Globals.h"

Scene::Scene()
{
	player = nullptr;
    level = nullptr;
	
	camera.target = { 0, 0 };				//Center of the screen
	camera.offset = { SIDE_MARGINS, TOP_MARGIN };	//Offset from the target (center of the screen)
	camera.rotation = 0.0f;					//No rotation
	camera.zoom = 1.0f;						//Default zoom

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
	//Create player
	player = new Player({ 0,0 }, State::IDLE, Look::RIGHT);
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

    return AppStatus::OK;
}
AppStatus Scene::LoadLevel(int stage)
{
	int size;
	int x, y, i;
	Tile tile;
	Point pos;
	int *map = nullptr;
	Object *obj = nullptr;
	
	ClearLevel();

	size = LEVEL_WIDTH * LEVEL_HEIGHT;
	if (stage == 1)
	{
		map = new int[size] {
			  1,  2,  3,  4,  1,  2,  3,  4,  1,  2,  3,  4,  1,  2,  3,  4,
			  5,  6,  7,  8,  5,  6,  7,  8,  5,  6,  7,  8,  5,  6,  7,  8,
			  9, 10, 11, 12,  9, 10, 11, 12,  9, 10, 11, 12,  9, 10, 11, 12,
			 13, 14, 15, 16, 13, 14, 15, 16, 13, 14, 15, 16, 13, 14, 15, 16,
			 17, 18, 19, 20, 17, 18, 19, 20, 17, 18, 19, 20, 17, 18, 19, 20,
			 21, 22, 23, 24, 25, 26, 27, 28, 21, 26, 27, 28, 25, 26, 27, 28,
			 29, 30, 41, 42, 31, 32, 29, 32, 29, 32, 29, 33, 34, 32, 29, 32,
			 36, 35, 43, 44, 35, 35, 36, 35, 36, 35, 36, 35, 35, 35, 36, 35,
			 37, 37, 45, 46, 38, 37, 37, 37, 37, 37, 37, 37, 38, 37, 37, 37,
			 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
			 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40
		};
		player->InitScore();
	}
	else if (stage == 2)
	{
		map = new int[size] {
			  1,  2,  3,  4,  1,  2,  3,  4,  1,  2,  3,  4,  1,  2,  3,  4,
			  5,  6,  7,  8,  5,  6,  7,  8,  5,  6,  7,  8,  5,  6,  7,  8,
			  9, 10, 11, 12,  9, 10, 11, 12,  9, 10, 11, 12,  9, 10, 11, 12,
			 13, 14, 15, 16, 13, 14, 15, 16, 13, 14, 15, 16, 13, 14, 15, 16,
			 17, 18, 19, 20, 17, 18, 19, 20, 17, 18, 19, 20, 17, 18, 19, 20,
			 21, 22, 23, 24, 25, 26, 27, 28, 21, 26, 27, 28, 25, 26, 27, 28,
			 29, 30, 41, 42, 31, 32, 29, 32, 29, 32, 29, 33, 34, 32, 29, 32,
			 36, 35, 43, 44, 35, 35, 36, 35, 36, 35, 36, 35, 35, 35, 36, 35,
			 37, 37, 45, 46, 38, 37, 37, 37, 37, 37, 37, 37, 38, 37, 37, 37,
			 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
			 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40
		};
	}
	else if (stage == 3)
	{
		map = new int[size] {
			  1,  2,  3,  4,  1,  2,  3,  4,  1,  2, 47, 48, 49, 48, 49, 48,
			  5,  6,  7,  8,  5,  6,  7,  8,  5,  6, 50, 51, 52, 52, 53, 54,
			  9, 10, 11, 12,  9, 10, 11, 12,  9, 10, 55, 56, 57, 58, 53, 59,
			 13, 14, 15, 16, 13, 14, 15, 16, 13, 14, 50, 60, 61, 40, 62, 63,
			 17, 18, 19, 20, 17, 18, 19, 20, 17, 18, 55, 65, 66, 40, 67, 68,
			 21, 22, 23, 24, 25, 26, 27, 28, 21, 26, 50, 60, 61, 40, 62, 63,
			 29, 30, 41, 42, 31, 32, 29, 32, 29, 32, 55, 65, 66, 40, 67, 68,
			 36, 35, 43, 44, 35, 35, 36, 35, 36, 35, 69, 70, 71, 40, 72, 73,
			 37, 37, 45, 46, 38, 37, 37, 37, 37, 37, 69, 70, 71, 40, 72, 73,
			 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
			 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40
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
			++i;
		}
	}
	//Tile map
	level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);

	return AppStatus::OK;
}
void Scene::Update()
{
	Point p1, p2;
	AABB box;

	//Switch between the different debug modes: off, on (sprites & hitboxes), on (hitboxes) 
	if (IsKeyPressed(KEY_F1))
	{
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);
	}
	//Debug levels instantly
	if (IsKeyPressed(KEY_ONE))			LoadLevel(1);
	else if (IsKeyPressed(KEY_TWO))		LoadLevel(2);
	else if (IsKeyPressed(KEY_THREE))	LoadLevel(3);

	level->Update();
	player->Update();
	CheckCollisions();
}
void Scene::Render()
{
	BeginMode2D(camera);

    level->Render();
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
	{
		RenderObjects(); 
		player->Draw();
	}
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		RenderObjectsDebug(YELLOW);
		player->DrawDebug(GREEN);
	}

	EndMode2D();

	RenderGUI();
}
void Scene::Release()
{
    level->Release();
	player->Release();
	ClearLevel();
}
bool Scene::PlayerIsDead() const {
	return player->IsDead();
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
			player->IncrScore((*it)->Points());
			
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
	//Temporal approach
	DrawText(TextFormat("SCORE : %d", player->GetScore()), 10, 10, 8, LIGHTGRAY);
}