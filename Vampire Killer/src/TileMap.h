#pragma once
#include <raylib.h>
#include "Sprite.h"
#include "Point.h"
#include "AABB.h"
#include "Globals.h"
#include <unordered_map>

enum class Tile {

	// -1: area covered by an entity
	EMPTY = -1,
	//  0: air tile
	AIR = 0,

	// ------------------ Outside tileset ------------------ //

	// 1 <= id <= 34: tree tiles
	TREE_FIRST_1 = 1, TREE_FIRST_2, TREE_FIRST_3, TREE_FIRST_4,
	TREE_SECOND_1, TREE_SECOND_2, TREE_SECOND_3, TREE_SECOND_4,
	TREE_THIRD_1, TREE_THIRD_2, TREE_THIRD_3, TREE_THIRD_4,
	TREE_FOURTH_1, TREE_FOURTH_2, TREE_FOURTH_3, TREE_FOURTH_4,
	TREE_FIFTH_1, TREE_FIFTH_2, TREE_FIFTH_3, TREE_FIFTH_4,
	TREE_21, TREE_22, TREE_23, TREE_24, TREE_25, TREE_26, TREE_27,
	TREE_28, TREE_29, TREE_30, TREE_31, TREE_32, TREE_33, TREE_34,

	// 35 <= id <= 38: fences
	FENCE_TOP, FENCE_TOP_TREE,
	FENCE_BOTTOM, FENCE_BOTTOM_STATUE,

	// 39: grass floor
	GRASS_FLOOR,

	// 40: void
	VOID_SPACE,

	// 41 <= id <= 46: statue
	STATUE_TOP_LEFT, STATUE_TOP_RIGHT,
	STATUE_MIDDLE_LEFT, STATUE_MIDDLE_RIGHT,
	STATUE_BOTTOM_LEFT, STATUE_BOTTOM_RIGHT,

	// 47 <= id <= 73: castle entrance
	CASTLE_TOP_1, CASTLE_TOP_2, CASTLE_TOP_3,
	CASTLE_FIRST_1, CASTLE_FIRST_2, CASTLE_FIRST_3, CASTLE_FIRST_4, CASTLE_FIRST_5,
	CASTLE_SECOND_1, CASTLE_SECOND_2, CASTLE_SECOND_3, CASTLE_SECOND_4, CASTLE_SECOND_5,
	CASTLE_THIRD_1, CASTLE_THIRD_2, CASTLE_THIRD_3, CASTLE_THIRD_4,
	CASTLE_FOURTH_1, CASTLE_FOURTH_2, CASTLE_FOURTH_3, CASTLE_FOURTH_4, CASTLE_FOURTH_5,
	CASTLE_GREY_1, CASTLE_GREY_2, CASTLE_GREY_3, CASTLE_GREY_4, CASTLE_GREY_5,

	/*---------Level 1---------*/

	// 74 <= id <= 78: columns
	COLUMN_TOP, COLUMN_MID, COLUMN_BOT, COLUMN_BROKE_DOWN, COLUMN_BROKE_UP,

	// 79 <= id <= 89: curtain
	CURTAIN_TOP_LEFT, CURTAIN_TOP_RIGHT, CURTAIN_MID_LEFT, CURTAIN_MID_RIGHT, CURTAIN_BOT_LEFT,
	CURTAIN_BOT_RIGHT, CURTAIN_STRAIGHT_MOST_LIGHT, CURTAIN_STRAIGHT_LIGHT, CURTAIN_STRAIGHT_LESS_LIGHT,
	CURTAIN_STRAIGHT_ALMOST_END, CURTAIN_STRAIGHT_END,

	// 90 <= id <= 99: decomposed wall
	LEVEL1_WALL_0, LEVEL1_WALL_1, LEVEL1_WALL_2, LEVEL1_WALL_3, LEVEL1_WALL_4, LEVEL1_WALL_5, 
	LEVEL1_WALL_6, LEVEL1_WALL_7, LEVEL1_WALL_8, LEVEL1_WALL_9,

	// 100: window
	WINDOW,

	// 101 <= id <= 102: wall stairs
	WALL_STAIRS_CORNER, WALL_STAIRS,

	// 103 <= id <= 104: floor
	LEVEL1_FLOOR_1, LEVEL1_FLOOR_2,

	// 105: wall
	WALL_DEFAULT,

	// 106 <= id <= 113: castle wall
	LEVEL1_WALL_CASTLE_0, LEVEL1_WALL_CASTLE_1, LEVEL1_WALL_CASTLE_2, LEVEL1_WALL_CASTLE_3, 
	LEVEL1_WALL_CASTLE_4, LEVEL1_WALL_CASTLE_5, LEVEL1_WALL_CASTLE_6, LEVEL1_WALL_CASTLE_7,

	// 114 <= id <= 115: castle stairs
	WALL_CASTLE_STAIRS_CORNER, WALL_CASTLE_STAIRS,

	//116: prison cage
	PRISON,

	// id >= 200: entities' initial locations
	PLAYER = 200,

	//Intervals
	STATIC_FIRST = TREE_FIRST_1,
	STATIC_LAST = CASTLE_GREY_5,
	SOLID_FIRST = GRASS_FLOOR,
	SOLID_LAST = GRASS_FLOOR,
	ENTITY_FIRST = PLAYER,
	ENTITY_LAST = PLAYER
};

class TileMap
{
public:
	TileMap();
	~TileMap();

	AppStatus Initialise();
	AppStatus Load(int data[], int dataFront[], int dataBack[], int w, int h);
	void Update();
	void RenderEarly();
	void Render();
	void RenderLate();
	void Release();

	//Test for collisions with walls
	bool TestCollisionWallLeft(const AABB& box) const;
	bool TestCollisionWallRight(const AABB& box) const;
	
	//Test collision with the ground and update 'py' with the maximum y-position to prevent
	//penetration of the grounded tile, that is, the pixel y-position above the grounded tile.
	//Grounded tile = solid tile (blocks) or ladder tops.
	bool TestCollisionGround(const AABB& box, int *py) const;
	
	//Test if there is a ground tile one pixel below the given box
	bool TestFalling(const AABB& box) const;

private:
	void InitTileDictionary();

	Tile GetBackTileIndex(int x, int y) const;
	Tile GetTileIndex(int x, int y) const;
	Tile GetFrontTileIndex(int x, int y) const;
	bool IsTileSolid(Tile tile) const;
	bool CollisionX(const Point& p, int distance) const;
	bool CollisionY(const Point& p, int distance) const;

	//Tile map
	Tile *mapBack;
	Tile *map;
	Tile *mapFront;

	//Size of the tile map
	int size, width, height;
	
	//Dictionary of tile frames
	std::unordered_map<int, Rectangle> dict_rect;
	
	//Tile sheet
	const Texture2D *img_tiles;
};

