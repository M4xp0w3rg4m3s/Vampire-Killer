#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 32x32
#define ATTACK_FRAME_WIDTH	80
#define ATTACK_FRAME_HEIGHT	64

//Logical model size: 12x28
#define WHIP_WIDTH	12
#define WHIP_HEIGHT	28

#define CHAIN_WIDTH	12
#define CHAIN_HEIGHT	28

//Frame delay for updating the attack velocity
#define PLAYER_ATTACK_DELAY		6

//Rendering states
enum class WeaponAnim {
	WHIP_1_RIGHT, WHIP_2_RIGHT, WHIP_3_RIGHT,
	WHIP_1_LEFT, WHIP_2_LEFT, WHIP_3_LEFT,
	CHAIN_1_RIGHT, CHAIN_2_RIGHT, CHAIN_3_RIGHT,
	CHAIN_1_LEFT, CHAIN_2_LEFT, CHAIN_3_LEFT,
	NOTHING_AT_ALL,

	NUM_ANIMATIONS
};

enum class LookAt { RIGHT, LEFT };

enum class WeaponType {
	WHIP, CHAIN
};

class Weapon : public Entity
{
public:
	Weapon(const Point& p);
	~Weapon();

	AppStatus Initialise();

	void Update(Point position, bool crouching);
	void SetWeapon(WeaponType type);
	void DrawDebug(const Color& col) const;
	void Release();

	void Attack(int frame, LookAt looking);

private:

	void AttackRight(int frame);
	void AttackLeft(int frame);

	//Animation management
	void SetAnimation(int id);
	WeaponAnim GetAnimation() const;

	WeaponType currentWeapon;
	LookAt currentLooking;

	const Point attackOffsetRight = {8, 48};
	const Point attackOffsetLeft = {-8, 48 };

	const Point attackOffsetRightCrouch = { 8, 54 };
	const Point attackOffsetLeftCrouch = { -8, 54 };
};