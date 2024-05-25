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

	// ------------------ Level 1 ------------------ //

	// 74 <= id <= 81: columns
	COLUMN_TOP_RIGHT, COLUMN_TOP_LEFT,
	COLUMN_MIDDLE_RIGHT, COLUMN_MIDDLE_LEFT,
	COLUMN_BROKE_DOWN,
	COLUMN_BROKE_UP,
	COLUMN_BOTTOM_RIGHT, COLUMN_BOTTOM_LEFT,

	// 82 <= id <= 92: curtain
	CURTAIN_TOP_LEFT, CURTAIN_TOP_RIGHT,
	CURTAIN_MIDDLE_LEFT, CURTAIN_MIDDLE_RIGHT,
	CURTAIN_BOTTOM_LEFT, CURTAIN_BOTTOM_RIGHT,
	CURTAIN_STRAIGHT_TOP,
	CURTAIN_STRAIGHT_MIDDLE_LIGHT,
	CURTAIN_STRAIGHT_MIDDLE,
	CURTAIN_STRAIGHT_BOTTOM,
	CURTAIN_STRAIGHT_END,

	// 93 <= id <= 102: decomposed wall
	WORN_WALL_1, WORN_WALL_2,
	WORN_WALL_TOP_RIGHT, WORN_WALL_TOP_LEFT,
	WORN_WALL_FIRST_RIGHT, WORN_WALL_FIRST_LEFT,
	WORN_WALL_SECOND_RIGHT, WORN_WALL_SECOND_LEFT,
	WORN_WALL_BOTTOM_RIGHT, WORN_WALL_BOTTOM_LEFT,

	// 103: window
	WINDOW,

	// 104 <= id <= 107: wall stairs
	WHITE_STAIRS_CORNER_RIGHT, WHITE_STAIRS_CORNER_LEFT,
	WHITE_STAIRS_LEFT, WHITE_STAIRS_RIGHT,

	// 108 <= id <= 109: floor
	BRICK_FLOOR_1, BRICK_FLOOR_2,

	// 110: wall
	WHITE_WALL,

	// 111 <= id <= 118: castle wall
	BOSS_WALL,
	BOSS_HOLE_1,
	BOSS_HOLE_2,
	BOSS_HOLE_3_TOP_LEFT, BOSS_HOLE_3_TOP_RIGHT,
	BOSS_HOLE_4,
	BOSS_HOLE_CORNER_RIGHT, BOSS_HOLE_CORNER_LEFT,

	// 119 <= id <= 122: castle stairs
	BOSS_STAIRS_RIGHT, BOSS_STAIRS_LEFT,
	BOSS_STAIRS_CORNER_RIGHT, BOSS_STAIRS_CORNER_LEFT,

	// 123: boss fence
	BOSS_FENCE,

	// 124 <= : special tiles
	FIRE = 124, FIRE_FRAME1, FIRE_FRAME2,
	CANDLE = 126, CANDLE_FRAME1, CANDLE_FRAME2,
	CHAIN,

	// 200 =< id: entities' initial locations
	PLAYER = 200,

	// 500 =< id: end invisible teletransportating walls
	LEFT = 500, RIGHT,TOP,BOTTOM,
	WIN = 505,

	// 550 = solid invisible tile
	INVISIBLE = 550,

	//Intervals
	STATIC_FIRST = TREE_FIRST_1,
	STATIC_LAST = BOSS_FENCE,
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
	bool TestCollisionLeft(const AABB& box) const;
	bool TestCollisionRight(const AABB& box) const;
	bool TestCollisionTop(const AABB& box) const;
	bool TestCollisionBottom(const AABB& box) const;
	bool TestCollisionWin(const AABB& box) const;

	
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
	
	Sprite* fire;
	Sprite* candle;

	//Tile sheet
	const Texture2D *img_tiles;
};

