
#include "Weapon.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Weapon::Weapon(const Point& p) :
	Entity(p, WHIP_WIDTH, WHIP_HEIGHT, ATTACK_FRAME_WIDTH, ATTACK_FRAME_WIDTH)
{
	Initialise();
	currentWeapon = WeaponType::WHIP;
	SetHitbox({ pos.x + 15, pos.y - 63 }, width, height);
}
Weapon::~Weapon()
{
}
AppStatus Weapon::Initialise()
{
	int i;
	const int n = ATTACK_FRAME_WIDTH;
	const int n2 = ATTACK_FRAME_HEIGHT;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_WEAPON, "images/Spritesheets/Simon/WhipChainAttacks.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(Resource::IMG_WEAPON));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)WeaponAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)WeaponAnim::WHIP_1_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)WeaponAnim::WHIP_1_RIGHT, { 0, 0, n, n2 });
	sprite->SetAnimationDelay((int)WeaponAnim::WHIP_2_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)WeaponAnim::WHIP_2_RIGHT, { n, 0, n, n2 });
	sprite->SetAnimationDelay((int)WeaponAnim::WHIP_3_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)WeaponAnim::WHIP_3_RIGHT, { 2 * n, 0, n, n2 });

	sprite->SetAnimationDelay((int)WeaponAnim::WHIP_1_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)WeaponAnim::WHIP_1_LEFT, { 0, 0, -n, n2 });
	sprite->SetAnimationDelay((int)WeaponAnim::WHIP_2_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)WeaponAnim::WHIP_2_LEFT, { n, 0, -n, n2 });
	sprite->SetAnimationDelay((int)WeaponAnim::WHIP_3_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)WeaponAnim::WHIP_3_LEFT, { 2 * n, 0, -n, n2 });

	sprite->SetAnimationDelay((int)WeaponAnim::CHAIN_1_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)WeaponAnim::CHAIN_1_RIGHT, { 0, n2, n, n2 });
	sprite->SetAnimationDelay((int)WeaponAnim::CHAIN_2_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)WeaponAnim::CHAIN_2_RIGHT, { n, n2, n, n2 });
	sprite->SetAnimationDelay((int)WeaponAnim::CHAIN_3_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)WeaponAnim::CHAIN_3_RIGHT, { 2 * n, n2, n, n2 });

	sprite->SetAnimationDelay((int)WeaponAnim::CHAIN_1_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)WeaponAnim::CHAIN_1_LEFT, { n, 0, -n, n2 });
	sprite->SetAnimationDelay((int)WeaponAnim::CHAIN_2_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)WeaponAnim::CHAIN_2_LEFT, { n, n2, -n, n2 });
	sprite->SetAnimationDelay((int)WeaponAnim::CHAIN_3_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)WeaponAnim::CHAIN_3_LEFT, { 2 * n, n2, -n, n2 });

	sprite->SetAnimationDelay((int)WeaponAnim::NOTHING_AT_ALL, ANIM_DELAY);
	sprite->AddKeyFrame((int)WeaponAnim::NOTHING_AT_ALL, { 0, 0, 0, 0 });

	return AppStatus::OK;
}
void Weapon::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
WeaponAnim Weapon::GetAnimation() const
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (WeaponAnim)sprite->GetAnimation();
}
void Weapon::Update(Point position, bool crouching)
{
	if (currentLooking == LookAt::RIGHT) {
		if (crouching) {
			SetPos(position + attackOffsetRightCrouch);
		}
		else {
			SetPos(position + attackOffsetRight);
		}
	}
	else {
		if (crouching) {
			SetPos(position + attackOffsetLeftCrouch);
		}
		else {
			SetPos(position + attackOffsetLeft);
		}
	}
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void Weapon::SetWeapon(WeaponType type) {
	currentWeapon = type;
}
void Weapon::Attack(int frame, LookAt looking) {
	currentLooking = looking;
	if (looking == LookAt::RIGHT) {
		AttackRight(frame);
	}
	else {
		AttackLeft(frame);
	}
}

void Weapon::AttackRight(int frame) {
	if (frame == 0) {
		currentFrame = 1;
		if (currentWeapon == WeaponType::WHIP) {
			SetAnimation((int)WeaponAnim::WHIP_1_RIGHT);
		}
		else if (currentWeapon == WeaponType::CHAIN) {
			SetAnimation((int)WeaponAnim::CHAIN_1_RIGHT);
		}
	}
	else if (frame == 1) {
		currentFrame = 1;
		if (currentWeapon == WeaponType::WHIP) {
				SetAnimation((int)WeaponAnim::WHIP_2_RIGHT);
		}
		else if (currentWeapon == WeaponType::CHAIN) {
			SetAnimation((int)WeaponAnim::CHAIN_2_RIGHT);
		}
	}
	else if (frame == 2) {
		currentFrame = 2;
		if (currentWeapon == WeaponType::WHIP) {
			SetAnimation((int)WeaponAnim::WHIP_3_RIGHT);
		}
		else if (currentWeapon == WeaponType::CHAIN) {
			SetAnimation((int)WeaponAnim::CHAIN_3_RIGHT);
		}
	}
	else {
		currentFrame = -1;
		SetAnimation((int)WeaponAnim::NOTHING_AT_ALL);
	}

}
void Weapon::AttackLeft(int frame) {
	if (frame == 0) {
		currentFrame = 0;
		if (currentWeapon == WeaponType::WHIP)
			SetAnimation((int)WeaponAnim::WHIP_1_LEFT);
		else if (currentWeapon == WeaponType::CHAIN) {
			SetAnimation((int)WeaponAnim::CHAIN_1_LEFT);
		}
	}
	else if (frame == 1) {
		currentFrame = 1;
		if (currentWeapon == WeaponType::WHIP)
			SetAnimation((int)WeaponAnim::WHIP_2_LEFT);
		else if (currentWeapon == WeaponType::CHAIN) {
			SetAnimation((int)WeaponAnim::CHAIN_2_LEFT);
		}
	}
	else if (frame == 2) {
		currentFrame = 2;
		if (currentWeapon == WeaponType::WHIP)
			SetAnimation((int)WeaponAnim::WHIP_3_LEFT);
		else if (currentWeapon == WeaponType::CHAIN) {
			SetAnimation((int)WeaponAnim::CHAIN_3_LEFT);
		}
	}
	else {
		currentFrame = -1;
		SetAnimation((int)WeaponAnim::NOTHING_AT_ALL);
	}
}

void Weapon::DrawDebug(const Color& col) const
{
	if (currentFrame == 2) {
		if (currentWeapon == WeaponType::WHIP) {
			if (currentLooking == LookAt::RIGHT) {
				Entity::DrawHitbox(pos.x + 15, pos.y - 63, width, height, col);
			}
			else {
				Entity::DrawHitbox(pos.x - 15, pos.y - 63, width, height, col);
			}
		}
		else {
			if (currentLooking == LookAt::RIGHT) {
				Entity::DrawHitbox(pos.x + 15, pos.y - 63, width + 7, height, col);
			}
			else {
				Entity::DrawHitbox(pos.x - 15, pos.y - 63, width - 7, height, col);
			}
		}
	}
}
void Weapon::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_WEAPON);

	render->Release();
}