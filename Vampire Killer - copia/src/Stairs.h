#pragma once
#include "Entity.h"
#include "TileMap.h"
#include "Player.h"

class Stair
{
public:
	Stair(Vector2 fromLeftPoint, Vector2 toRightPoint);
	~Stair();

	AppStatus Initialise();

	void Update();

	void Reset();

	void DrawDebug(const Color& col) const;

protected:
	Vector2 point1;
	Vector2 point2;

};

