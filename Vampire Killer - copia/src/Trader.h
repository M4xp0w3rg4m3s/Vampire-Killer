#pragma once
#include "Entity.h"
#include "TileMap.h"
#include "AudioPlayer.h"


#define TRADER_SPRITE_HEIGHT	32
#define TRADER_SPRITE_WIDTH		32

#define TRADER_HITBOX_HEIGHT	16
#define TRADER_HITBOX_WIDTH		32

enum class TraderAnim {
	WHITE_TRADER, RED_TRADER,
	PINK_TRADER, PURPLE_TRADER, BLUE_TRADER, GRAY_TRADER, ADVANCING_RIGHT,
	EMPTY,
	NUM_ANIMATIONS
};

class Trader : public Entity
{
public:
	Trader(Point pos);
	~Trader();

	AppStatus Initialise();

	void Update();
	void Render();

	void Reset();
	void Brain();

	void SetTileMap(TileMap* tilemap);

	void DrawDebug(const Color& col) const;
	void Release();

	void SetAnimation(int id);

private:

	TileMap* map;

};

int AnimationFrame;