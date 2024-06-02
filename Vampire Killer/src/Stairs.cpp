
#include "Stairs.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Stair::Stair(Vector2 fromLeftPoint, Vector2 toRightPoint)
{
	point1 = fromLeftPoint;
	point2 = toRightPoint;
}
Stair::~Stair()
{
}
AppStatus Stair::Initialise()
{
	return AppStatus::OK;
}
void Stair::Update()
{
}
void Stair::Reset()
{
}
void Stair::DrawDebug(const Color& col) const
{
}

