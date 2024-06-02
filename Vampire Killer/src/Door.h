#pragma once
#include "Entity.h"
#include "Weapon.h"
#include "TileMap.h"
#include "Player.h"

//Logic states
enum class DoorState {
	OPEN, CLOSED
};

class Door : public Entity
{
public:
	Door(Point pos);
	~Door();

	AppStatus Initialise();

	void Update();
	void Render();

	void Open();

	void DrawDebug(const Color& col) const;
	void Release();

protected:

	DoorState state;
};

