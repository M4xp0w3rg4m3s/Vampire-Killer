
#include "Door.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>
#include "EnemyManager.h"
#include "StaticImage.h"

Door::Door(Point pos) : Entity(pos, 6, 48, 6, 48)
{
	state = DoorState::CLOSED;
	Initialise();
}
Door::~Door()
{
}
AppStatus Door::Initialise()
{
	int i;
	const float n = 16;
	Rectangle rc;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_DOOR, "images/Spritesheets/FX/DoorSprite.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	rc = { 0,0, n, n*3 };
	render = new StaticImage(data.GetTexture(Resource::IMG_DOOR), rc);


	return AppStatus::OK;
}
void Door::Update()
{
	if (state == DoorState::OPEN) {
		Open();
	}
	else {
	}
}
void Door::Render()
{
	Draw();
}
void Door::Open()
{
	state = DoorState::OPEN;
	pos.y--;
}
void Door::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
void Door::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_DOOR);

	render->Release();
}