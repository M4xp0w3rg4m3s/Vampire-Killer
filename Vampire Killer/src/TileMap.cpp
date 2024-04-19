#include "TileMap.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <cstring>

TileMap::TileMap()
{
	map = nullptr;
	mapFront = nullptr;
	mapBack = nullptr;
	width = 0;
	height = 0;
	candle = nullptr;
	fire = nullptr;
	img_tiles = nullptr;

	InitTileDictionary();
}
TileMap::~TileMap()
{
	if (map != nullptr && mapFront != nullptr && mapBack != nullptr)
	{
		delete[] map;
		delete[] mapFront;
		delete[] mapBack;
		map = nullptr;
		mapFront = nullptr;
		mapBack = nullptr;
	}
	if (fire != nullptr)
	{
		fire->Release();
		delete fire;
		fire = nullptr;
	}
	if (candle != nullptr)
	{
		candle->Release();
		delete candle;
		candle = nullptr;
	}
}
void TileMap::InitTileDictionary()
{
	const int n = TILE_SIZE;

	/* Level Intro */
	dict_rect[(int)Tile::TREE_FIRST_1] = { 0,  0, n, n };
	dict_rect[(int)Tile::TREE_FIRST_2] = { n,  0, n, n };
	dict_rect[(int)Tile::TREE_FIRST_3] = { 2 * n, 0, n, n };
	dict_rect[(int)Tile::TREE_FIRST_4] = { 3 * n, 0, n, n };

	dict_rect[(int)Tile::TREE_SECOND_1] = { 4 * n, 0, n, n };
	dict_rect[(int)Tile::TREE_SECOND_2] = { 5 * n, 0, n, n };
	dict_rect[(int)Tile::TREE_SECOND_3] = { 6 * n, 0, n, n };
	dict_rect[(int)Tile::TREE_SECOND_4] = { 7 * n, 0, n, n };

	dict_rect[(int)Tile::TREE_THIRD_1] = { 8 * n, 0, n, n };
	dict_rect[(int)Tile::TREE_THIRD_2] = { 9 * n, 0, n, n };
	dict_rect[(int)Tile::TREE_THIRD_3] = { 10 * n, 0, n, n };
	dict_rect[(int)Tile::TREE_THIRD_4] = { 11 * n, 0, n, n };

	dict_rect[(int)Tile::TREE_FOURTH_1] = { 12 * n, 0, n, n };
	dict_rect[(int)Tile::TREE_FOURTH_2] = { 13 * n, 0, n, n };
	dict_rect[(int)Tile::TREE_FOURTH_3] = { 14 * n, 0, n, n };
	dict_rect[(int)Tile::TREE_FOURTH_4] = { 15 * n, 0, n, n };

	dict_rect[(int)Tile::TREE_FIFTH_1] = { 0, n, n, n };
	dict_rect[(int)Tile::TREE_FIFTH_2] = { n, n, n, n };
	dict_rect[(int)Tile::TREE_FIFTH_3] = { 2 * n, n, n, n };
	dict_rect[(int)Tile::TREE_FIFTH_4] = { 3 * n, n, n, n };

	dict_rect[(int)Tile::TREE_21] = { 4 * n, n, n, n };
	dict_rect[(int)Tile::TREE_22] = { 5 * n, n, n, n };
	dict_rect[(int)Tile::TREE_23] = { 6 * n, n, n, n };
	dict_rect[(int)Tile::TREE_24] = { 7 * n, n, n, n };
	dict_rect[(int)Tile::TREE_25] = { 8 * n, n, n, n };
	dict_rect[(int)Tile::TREE_26] = { 9 * n, n, n, n };
	dict_rect[(int)Tile::TREE_27] = { 10 * n, n, n, n };
	dict_rect[(int)Tile::TREE_28] = { 11 * n, n, n, n };
	dict_rect[(int)Tile::TREE_29] = { 12 * n, n, n, n };
	dict_rect[(int)Tile::TREE_30] = { 13 * n, n, n, n };
	dict_rect[(int)Tile::TREE_31] = { 14 * n, n, n, n };
	dict_rect[(int)Tile::TREE_32] = { 15 * n, n, n, n };
	dict_rect[(int)Tile::TREE_33] = { 0, 2 * n, n, n };
	dict_rect[(int)Tile::TREE_34] = { n, 2 * n, n, n };

	dict_rect[(int)Tile::FENCE_TOP_TREE] = { 2 * n, 2 * n, n, n };
	dict_rect[(int)Tile::FENCE_TOP] = { 3 * n, 2 * n, n, n };
	dict_rect[(int)Tile::FENCE_BOTTOM] = { 4 * n, 2 * n, n, n };
	dict_rect[(int)Tile::FENCE_BOTTOM_STATUE] = { 5 * n, 2 * n, n, n };

	dict_rect[(int)Tile::GRASS_FLOOR] = { 6 * n, 2 * n, n, n };

	dict_rect[(int)Tile::VOID_SPACE] = { 7 * n, 2 * n, n, n };

	dict_rect[(int)Tile::STATUE_TOP_LEFT] = { 8 * n, 2 * n, n, n };
	dict_rect[(int)Tile::STATUE_TOP_RIGHT] = { 9 * n, 2 * n, n, n };
	dict_rect[(int)Tile::STATUE_MIDDLE_LEFT] = { 10 * n, 2 * n, n, n };
	dict_rect[(int)Tile::STATUE_MIDDLE_RIGHT] = { 11 * n, 2 * n, n, n };
	dict_rect[(int)Tile::STATUE_BOTTOM_LEFT] = { 12 * n, 2 * n, n, n };
	dict_rect[(int)Tile::STATUE_BOTTOM_RIGHT] = { 13 * n, 2 * n, n, n };

	dict_rect[(int)Tile::CASTLE_TOP_1] = { 14 * n, 2 * n, n, n };
	dict_rect[(int)Tile::CASTLE_TOP_2] = { 15 * n, 2 * n, n, n };
	dict_rect[(int)Tile::CASTLE_TOP_3] = { 0 * n, 3 * n, n, n };

	dict_rect[(int)Tile::CASTLE_FIRST_1] = { 1 * n, 3 * n, n, n };
	dict_rect[(int)Tile::CASTLE_FIRST_2] = { 2 * n, 3 * n, n, n };
	dict_rect[(int)Tile::CASTLE_FIRST_3] = { 3 * n, 3 * n, n, n };
	dict_rect[(int)Tile::CASTLE_FIRST_4] = { 4 * n, 3 * n, n, n };
	dict_rect[(int)Tile::CASTLE_FIRST_5] = { 5 * n, 3 * n, n, n };

	dict_rect[(int)Tile::CASTLE_SECOND_1] = { 6 * n, 3 * n, n, n };
	dict_rect[(int)Tile::CASTLE_SECOND_2] = { 7 * n, 3 * n, n, n };
	dict_rect[(int)Tile::CASTLE_SECOND_3] = { 8 * n, 3 * n, n, n };
	dict_rect[(int)Tile::CASTLE_SECOND_4] = { 9 * n, 3 * n, n, n };
	dict_rect[(int)Tile::CASTLE_SECOND_5] = { 10 * n, 3 * n, n, n };

	dict_rect[(int)Tile::CASTLE_THIRD_1] = { 11 * n, 3 * n, n, n };
	dict_rect[(int)Tile::CASTLE_THIRD_2] = { 12 * n, 3 * n, n, n };
	dict_rect[(int)Tile::CASTLE_THIRD_3] = { 13 * n, 3 * n, n, n };
	dict_rect[(int)Tile::CASTLE_THIRD_4] = { 14 * n, 3 * n, n, n };

	dict_rect[(int)Tile::CASTLE_FOURTH_1] = { 15 * n, 3 * n, n, n };
	dict_rect[(int)Tile::CASTLE_FOURTH_2] = { 0 * n, 4 * n, n, n };
	dict_rect[(int)Tile::CASTLE_FOURTH_3] = { 1 * n, 4 * n, n, n };
	dict_rect[(int)Tile::CASTLE_FOURTH_4] = { 2 * n, 4 * n, n, n };
	dict_rect[(int)Tile::CASTLE_FOURTH_5] = { 3 * n, 4 * n, n, n };

	dict_rect[(int)Tile::CASTLE_GREY_1] = { 4 * n, 4 * n, n, n };
	dict_rect[(int)Tile::CASTLE_GREY_2] = { 5 * n, 4 * n, n, n };
	dict_rect[(int)Tile::CASTLE_GREY_3] = { 6 * n, 4 * n, n, n };
	dict_rect[(int)Tile::CASTLE_GREY_4] = { 7 * n, 4 * n, n, n };
	dict_rect[(int)Tile::CASTLE_GREY_5] = { 8 * n, 4 * n, n, n };

	/*Special Tiles*/
	dict_rect[(int)Tile::FIRE_FRAME1] = { 11 * n, 4 * n, n, n };
	dict_rect[(int)Tile::FIRE_FRAME2] = { 12 * n, 4 * n, n, n };

	dict_rect[(int)Tile::CANDLE_FRAME1] = { 9 * n, 4 * n, n, n };
	dict_rect[(int)Tile::CANDLE_FRAME2] = { 10 * n, 4 * n, n, n };

	/* Level 1 */
	dict_rect[(int)Tile::COLUMN_TOP_RIGHT] = { 0 * n, 5 * n, n, n };
	dict_rect[(int)Tile::COLUMN_TOP_LEFT] = { 0 * n, 5 * n, -n, n };
	dict_rect[(int)Tile::COLUMN_MIDDLE_RIGHT] = { 1 * n, 5 * n, n, n };
	dict_rect[(int)Tile::COLUMN_MIDDLE_LEFT] = { 1 * n, 5 * n, -n, n };
	dict_rect[(int)Tile::COLUMN_BROKE_DOWN] = { 2 * n, 5 * n, n, n };
	dict_rect[(int)Tile::COLUMN_BROKE_UP] = { 3 * n, 5 * n, n, n };
	dict_rect[(int)Tile::COLUMN_BOTTOM_RIGHT] = { 4 * n, 5 * n, n, n };
	dict_rect[(int)Tile::COLUMN_BOTTOM_LEFT] = { 4 * n, 5 * n, -n, n };

	dict_rect[(int)Tile::CURTAIN_TOP_LEFT] = { 5 * n, 5 * n, n, n };
	dict_rect[(int)Tile::CURTAIN_TOP_RIGHT] = { 6 * n, 5 * n, n, n };
	dict_rect[(int)Tile::CURTAIN_MIDDLE_LEFT] = { 7 * n, 5 * n, n, n };
	dict_rect[(int)Tile::CURTAIN_MIDDLE_RIGHT] = { 8 * n, 5 * n, n, n };
	dict_rect[(int)Tile::CURTAIN_BOTTOM_LEFT] = { 9 * n, 5 * n, n, n };
	dict_rect[(int)Tile::CURTAIN_BOTTOM_RIGHT] = { 10 * n, 5 * n, n, n };
	dict_rect[(int)Tile::CURTAIN_STRAIGHT_TOP] = { 11 * n, 5 * n, n, n };
	dict_rect[(int)Tile::CURTAIN_STRAIGHT_MIDDLE_LIGHT] = { 12 * n, 5 * n, n, n };
	dict_rect[(int)Tile::CURTAIN_STRAIGHT_MIDDLE] = { 13 * n, 5 * n, n, n };
	dict_rect[(int)Tile::CURTAIN_STRAIGHT_BOTTOM] = { 14 * n, 5 * n, n, n };
	dict_rect[(int)Tile::CURTAIN_STRAIGHT_END] = { 15 * n, 5 * n, n, n };

	dict_rect[(int)Tile::WORN_WALL_1] = { 0 * n, 6* n, n, n };
	dict_rect[(int)Tile::WORN_WALL_2] = { 1 * n, 6* n, n, n };
	dict_rect[(int)Tile::WORN_WALL_TOP_RIGHT] = { 2 * n, 6* n, n, n };
	dict_rect[(int)Tile::WORN_WALL_TOP_LEFT] = { 3 * n, 6* n, n, n };
	dict_rect[(int)Tile::WORN_WALL_FIRST_RIGHT] = { 4 * n, 6* n, n, n };
	dict_rect[(int)Tile::WORN_WALL_FIRST_LEFT] = { 5 * n, 6 * n, n, n };
	dict_rect[(int)Tile::WORN_WALL_SECOND_RIGHT] = { 6 * n, 6 * n, n, n };
	dict_rect[(int)Tile::WORN_WALL_SECOND_LEFT] = { 7 * n, 6 * n, n, n };
	dict_rect[(int)Tile::WORN_WALL_BOTTOM_RIGHT] = { 8 * n, 6 * n, n, n };
	dict_rect[(int)Tile::WORN_WALL_BOTTOM_LEFT] = { 9 * n, 6 * n, n, n };

	dict_rect[(int)Tile::WINDOW] = { 10 * n, 6 * n, n, n };

	dict_rect[(int)Tile::WHITE_STAIRS_CORNER_LEFT] = { 11 * n, 6 * n, n, n };
	dict_rect[(int)Tile::WHITE_STAIRS_CORNER_RIGHT] = { 11 * n, 6 * n, -n, n };
	dict_rect[(int)Tile::WHITE_STAIRS_LEFT] = { 12 * n, 6 * n, n, n };
	dict_rect[(int)Tile::WHITE_STAIRS_RIGHT] = { 12 * n, 6 * n, -n, n };

	dict_rect[(int)Tile::BRICK_FLOOR_1] = { 13 * n, 6 * n, n, n };
	dict_rect[(int)Tile::BRICK_FLOOR_2] = { 14 * n, 6 * n, n, n };

	dict_rect[(int)Tile::WHITE_WALL] = { 15 * n, 6 * n, n, n };

	dict_rect[(int)Tile::BOSS_WALL] = { 0 * n, 7 * n, n, n };
	dict_rect[(int)Tile::BOSS_HOLE_1] = { 1 * n, 7 * n, n, n };
	dict_rect[(int)Tile::BOSS_HOLE_2] = { 2 * n, 7 * n, n, n };
	dict_rect[(int)Tile::BOSS_HOLE_3_TOP_LEFT] = { 3 * n, 7 * n, n, n };
	dict_rect[(int)Tile::BOSS_HOLE_3_TOP_RIGHT] = { 4 * n, 7 * n, n, n };
	dict_rect[(int)Tile::BOSS_HOLE_4] = { 5 * n, 7 * n, n, n };
	dict_rect[(int)Tile::BOSS_HOLE_CORNER_RIGHT] = { 6 * n, 7 * n, n, n };
	dict_rect[(int)Tile::BOSS_HOLE_CORNER_LEFT] = { 7 * n, 7 * n, n, n };

	dict_rect[(int)Tile::BOSS_STAIRS_RIGHT] = { 8 * n, 7 * n, n, n };
	dict_rect[(int)Tile::BOSS_STAIRS_LEFT] = { 8 * n, 7 * n, -n, n };
	dict_rect[(int)Tile::BOSS_STAIRS_CORNER_RIGHT] = { 9 * n, 7 * n, n, n };
	dict_rect[(int)Tile::BOSS_STAIRS_CORNER_LEFT] = { 9 * n, 7 * n, -n, n };

	dict_rect[(int)Tile::BOSS_FENCE] = { 10 * n, 7 * n, n, n };
}
AppStatus TileMap::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(Resource::IMG_TILES, "images/Levels/LevelsTileset.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_tiles = data.GetTexture(Resource::IMG_TILES);

	fire = new Sprite(img_tiles);
	if (fire == nullptr)
	{
		LOG("Failed to allocate memory for fire sprite");
		return AppStatus::ERROR;
	}
	fire->SetNumberAnimations(1);
	fire->SetAnimationDelay(0, ANIM_DELAY);
	fire->AddKeyFrame(0, dict_rect[(int)Tile::FIRE_FRAME1]);
	fire->AddKeyFrame(0, dict_rect[(int)Tile::FIRE_FRAME2]);
	fire->SetAnimation(0);

	candle = new Sprite(img_tiles);
	if (candle == nullptr)
	{
		LOG("Failed to allocate memory for candle sprite");
		return AppStatus::ERROR;
	}
	candle->SetNumberAnimations(1);
	candle->SetAnimationDelay(0, ANIM_DELAY);
	candle->AddKeyFrame(0, dict_rect[(int)Tile::CANDLE_FRAME1]);
	candle->AddKeyFrame(0, dict_rect[(int)Tile::CANDLE_FRAME2]);
	candle->SetAnimation(0);

	return AppStatus::OK;
}
AppStatus TileMap::Load(int data[], int dataFront[], int dataBack[], int w, int h)
{
	size = w*h;
	width = w;
	height = h;

	if (mapBack != nullptr)	delete[] mapBack;

	mapBack = new Tile[size];
	if (mapBack == nullptr)
	{
		LOG("Failed to allocate memory for tile mapBack");
		return AppStatus::ERROR;
	}
	memcpy(mapBack, dataBack, size * sizeof(int));

	if (map != nullptr)	delete[] map;

	map = new Tile[size];
	if (map == nullptr)	
	{
		LOG("Failed to allocate memory for tile map");
		return AppStatus::ERROR;
	}
	memcpy(map, data, size * sizeof(int));

	if (mapFront != nullptr)	delete[] mapFront;

	mapFront = new Tile[size];
	if (mapFront == nullptr)
	{
		LOG("Failed to allocate memory for tile mapFront");
		return AppStatus::ERROR;
	}
	memcpy(mapFront, dataFront, size * sizeof(int));

	return AppStatus::OK;
}
void TileMap::Update()
{
	fire->Update();
	candle->Update();
}
Tile TileMap::GetBackTileIndex(int x, int y) const
{
	int idx = x + y * width;
	if (idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile mapBack dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y)
			return Tile::AIR;
	}
	return mapBack[x + y * width];
}
Tile TileMap::GetTileIndex(int x, int y) const
{
	int idx = x + y*width;
	if(idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y)
		return Tile::AIR;
	}
	return map[x + y * width];
}
Tile TileMap::GetFrontTileIndex(int x, int y) const
{
	int idx = x + y * width;
	if (idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile mapFront dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y)
			return Tile::AIR;
	}
	return mapFront[x + y * width];
}
bool TileMap::IsTileSolid(Tile tile) const
{
	return (tile == Tile::GRASS_FLOOR || tile == Tile::BRICK_FLOOR_1 || tile == Tile::BRICK_FLOOR_2 || tile == Tile::INVISIBLE);
}
bool TileMap::TestCollisionWallLeft(const AABB& box) const
{
	return CollisionX(box.pos + Point(-2, 0), box.height);
}
bool TileMap::TestCollisionWallRight(const AABB& box) const
{
	return CollisionX(box.pos + Point(box.width + 1, 0), box.height);
}
bool TileMap::TestCollisionLeft(const AABB& box) const
{
	const Point& p = box.pos;
	int distance = box.height;
	Tile tile;

	int x, y, y0, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = (p.x-3) / TILE_SIZE;
	y0 = p.y / TILE_SIZE;
	y1 = (p.y + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the vertical range
	for (y = y0; y <= y1; ++y)
	{
		tile = GetTileIndex(x, y);

		//One solid tile is sufficient
		if (tile == Tile::LEFT) {
			return true;
		}
	}
	return false;
}
bool TileMap::TestCollisionRight(const AABB& box) const
{
	const Point& p = box.pos;
	int distance = box.height;
	Tile tile;

	int x, y, y0, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = (p.x+14) / TILE_SIZE;
	y0 = p.y / TILE_SIZE;
	y1 = (p.y + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the vertical range
	for (y = y0; y <= y1; ++y)
	{
		tile = GetTileIndex(x, y);

		//One solid tile is sufficient
		if (tile == Tile::RIGHT) {
			return true;
		}
	}
	return false;
}
bool TileMap::TestCollisionGround(const AABB& box, int* py) const
{
	Point p(box.pos.x, *py);	//control point
	int tile_y;

	if (CollisionY(p, box.width))
	{
		tile_y = p.y / TILE_SIZE;

		*py = tile_y * TILE_SIZE - 1;
		return true;
	}
	return false;
}
bool TileMap::TestFalling(const AABB& box) const
{
	return !CollisionY(box.pos + Point(0, box.height), box.width);
}
bool TileMap::CollisionX(const Point& p, int distance) const
{
	int x, y, y0, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = (p.x) / TILE_SIZE;
	y0 = p.y / TILE_SIZE;
	y1 = (p.y + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the vertical range
	for (y = y0; y <= y1; ++y)
	{
		//One solid tile is sufficient
		if (IsTileSolid(GetTileIndex(x, y)))
			return true;
	}
	return false;
}
bool TileMap::CollisionY(const Point& p, int distance) const
{
	int x, y, x0, x1;
	Tile tile;

	//Calculate the tile coordinates and the range of tiles to check for collision
	y = p.y / TILE_SIZE;
	x0 = p.x / TILE_SIZE;
	x1 = (p.x + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the horizontal range
	for (x = x0; x <= x1; ++x)
	{
		tile = GetTileIndex(x, y);

		//One solid or laddertop tile is sufficient
		if (IsTileSolid(tile))
			return true;
	}
	return false;
}
void TileMap::RenderEarly()
{
	Tile tile;
	Rectangle rc;
	Vector2 pos;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			tile = mapBack[i * width + j];
			if (tile != Tile::AIR)
			{
				pos.x = (float)j * TILE_SIZE;
				pos.y = (float)i * TILE_SIZE;

				rc = dict_rect[(int)tile];
				DrawTextureRec(*img_tiles, rc, pos, WHITE);
			}
		}
	}
}
void TileMap::Render()
{
	Tile tile;
	Rectangle rc;
	Vector2 pos;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			tile = map[i * width + j];
			if (tile != Tile::AIR)
			{
				pos.x = (float)j * TILE_SIZE;
				pos.y = (float)i * TILE_SIZE;

				if (tile == Tile::FIRE){
					fire->Draw((int)pos.x, (int)pos.y);
				}
				if (tile == Tile::CANDLE){
					candle->Draw((int)pos.x, (int)pos.y);
				}
				else {
					rc = dict_rect[(int)tile];
					DrawTextureRec(*img_tiles, rc, pos, WHITE);
				}
				
			}
		}
	}
}
void TileMap::RenderLate()
{
	Tile tile;
	Rectangle rc;
	Vector2 pos;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			tile = mapFront[i * width + j];
			if (tile != Tile::AIR)
			{
				pos.x = (float)j * TILE_SIZE;
				pos.y = (float)i * TILE_SIZE;

				rc = dict_rect[(int)tile];
				DrawTextureRec(*img_tiles, rc, pos, WHITE);
			}
		}
	}
}
void TileMap::Release()
{
	ResourceManager& data = ResourceManager::Instance(); 
	data.ReleaseTexture(Resource::IMG_TILES);

	fire->Release();
	candle->Release();

	dict_rect.clear();
}