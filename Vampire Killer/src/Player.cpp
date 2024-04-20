
#include "Player.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include "Weapon.h"
#include <raymath.h>

Player::Player(const Point& p, State s, Look view) :
	Entity(p, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE)
{
	state = s;
	look = view;
	jump_delay = PLAYER_JUMP_DELAY;
	attack_delay = PLAYER_ATTACK_DELAY;
	die_delay = PLAYER_DYING_DELAY;
	map = nullptr;
	weapon = new Weapon(p);
	weapon->SetWeapon(WeaponType::WHIP);
	score = 0;
	AnimationFrame = 0;
	GodMode = false;
}
Player::~Player()
{
}
AppStatus Player::Initialise()
{
	int i;
	const int n = PLAYER_FRAME_SIZE;
	AnimationFrame = 0;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_PLAYER, "images/Spritesheets/Simon/Simon Spritesheet.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(Resource::IMG_PLAYER));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)PlayerAnim::NUM_ANIMATIONS);
	
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_RIGHT, { 0, 0, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_LEFT, { 0, 0, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_RIGHT, { (float)i*n, 0, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_LEFT, ANIM_DELAY);
	for (i = 0; i < 8; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_LEFT, { (float)i*n, 0, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_RIGHT, { n, n, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_RIGHT, { n, n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_LEFT, { 0, n, -n, n });
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_LEFT, { 0, n, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_RIGHT, { n, n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_LEFT, { n, n, -n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::LEVITATING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::LEVITATING_RIGHT, { n, n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::LEVITATING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::LEVITATING_LEFT, { n, n, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING, ANIM_LADDER_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::CLIMBING, { (float)i * n, 2 * n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_PRE_TOP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_PRE_TOP, { 0, 2 * n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_TOP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_TOP, { n, 2 * n, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_RIGHT, { (float)i * n, 5 * n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::ATTACKING_LEFT, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::ATTACKING_LEFT, { (float)i * n, 5 * n, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::CROUCHING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::CROUCHING_RIGHT, { 0, n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::CROUCHING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::CROUCHING_LEFT, { 0, n, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::CROUCH_ATTACK_RIGHT, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::CROUCH_ATTACK_RIGHT, { (float)i * n, 6 * n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::CROUCH_ATTACK_LEFT, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::CROUCH_ATTACK_LEFT, { (float)i * n, 6 * n, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::DYING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::DYING_RIGHT, { (float)i * n, 4 * n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::DYING_LEFT, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::DYING_LEFT, { (float)i * n, 4 * n, -n, n });

	sprite->SetAnimation((int)PlayerAnim::IDLE_RIGHT);

	AudioPlayer::Instance().CreateSound("audio/SFX/27.wav", "Attack");

	return AppStatus::OK;
}
void Player::InitScore()
{
	score = 0;
}
void Player::IncrScore(int n)
{
	score += n;
}
int Player::GetScore() const
{
	return score;
}
void Player::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
bool Player::IsLookingRight() const
{
	return look == Look::RIGHT;
}
bool Player::IsLookingLeft() const
{
	return look == Look::LEFT;
}
bool Player::IsAscending() const
{
	return dir.y < -PLAYER_LEVITATING_SPEED;
}
bool Player::IsLevitating() const
{
	return abs(dir.y) <= PLAYER_LEVITATING_SPEED;
}
bool Player::IsDescending() const
{
	return dir.y > PLAYER_LEVITATING_SPEED;
}
bool Player::IsInFirstHalfTile() const
{
	return pos.y % TILE_SIZE < TILE_SIZE / 2;
}
bool Player::IsInSecondHalfTile() const
{
	return pos.y % TILE_SIZE >= TILE_SIZE/2;
}
bool Player::IsGodMode() const {
	return GodMode;
}
bool Player::IsDead() const
{
	return state == State::DEAD;
}
bool Player::HasWon() const
{
	return state == State::WIN;
}
void Player::Win() {
	state = State::WIN;
}
void Player::GodModeSwitch() {
	if (GodMode == true) {
		GodMode = false;
	}
	else {
		GodMode = true;
	}
}
void Player::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
PlayerAnim Player::GetAnimation() const
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (PlayerAnim)sprite->GetAnimation();
}
void Player::Stop()
{
	dir = { 0,0 };
	state = State::IDLE;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::IDLE_RIGHT);
	else					SetAnimation((int)PlayerAnim::IDLE_LEFT);
}
void Player::StartWalkingLeft()
{
	state = State::WALKING;
	look = Look::LEFT;
	SetAnimation((int)PlayerAnim::WALKING_LEFT);
}
void Player::StartWalkingRight()
{
	state = State::WALKING;
	look = Look::RIGHT;
	SetAnimation((int)PlayerAnim::WALKING_RIGHT);
}
void Player::StartFalling()
{
	dir.y = PLAYER_SPEED;
	state = State::FALLING;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::FALLING_RIGHT);
	else					SetAnimation((int)PlayerAnim::FALLING_LEFT);
}
void Player::StartJumping()
{
	dir.y = -PLAYER_JUMP_FORCE;
	state = State::JUMPING;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::JUMPING_RIGHT);
	else					SetAnimation((int)PlayerAnim::JUMPING_LEFT);
	jump_delay = PLAYER_JUMP_DELAY;
}
void Player::StartWhip() {
	state = State::WHIP;
	weapon->SetWeapon(WeaponType::WHIP);
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::ATTACKING_RIGHT);
	else					SetAnimation((int)PlayerAnim::ATTACKING_LEFT);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
	attack_delay = PLAYER_ATTACK_DELAY;
	AnimationFrame = 0;
	if (IsLookingRight()) {
		weapon->Attack(AnimationFrame, (LookAt)Look::RIGHT);
	}
	else if (IsLookingLeft()) {
		weapon->Attack(AnimationFrame, (LookAt)Look::LEFT);
	}
	AudioPlayer::Instance().PlaySoundByName("Attack");
}
void Player::StartThrowing()
{
	state = State::THROWING;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::ATTACKING_RIGHT);
	else					SetAnimation((int)PlayerAnim::ATTACKING_LEFT);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
	attack_delay = PLAYER_ATTACK_DELAY;
}
void Player::StartCrouching()
{
	state = State::CROUCHING;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::CROUCHING_RIGHT);
	else					SetAnimation((int)PlayerAnim::CROUCHING_LEFT);
}
void Player::StartCrouchWhip() {
	state = State::CROUCH_WHIP;
	weapon->SetWeapon(WeaponType::WHIP);
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::CROUCH_ATTACK_RIGHT);
	else					SetAnimation((int)PlayerAnim::CROUCH_ATTACK_LEFT);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
	attack_delay = PLAYER_ATTACK_DELAY;
	AnimationFrame = 0;
	if (IsLookingRight()) {
		weapon->Attack(AnimationFrame, (LookAt)Look::RIGHT);
	}
	else if (IsLookingLeft()) {
		weapon->Attack(AnimationFrame, (LookAt)Look::LEFT);
	}
	AudioPlayer::Instance().PlaySoundByName("Attack");
}
void Player::StartCrouchThrowing()
{
	state = State::CROUCH_THROWING;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::CROUCH_ATTACK_RIGHT);
	else					SetAnimation((int)PlayerAnim::CROUCH_ATTACK_LEFT);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
	attack_delay = PLAYER_ATTACK_DELAY;
}
void Player::StartDying()
{
	state = State::DYING;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::DYING_RIGHT);
	else					SetAnimation((int)PlayerAnim::DYING_LEFT);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
	die_delay = PLAYER_DYING_DELAY;
}
void Player::StartClimbingUp()
{
	state = State::CLIMBING;
	SetAnimation((int)PlayerAnim::CLIMBING);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
}
void Player::StartClimbingDown()
{
	state = State::CLIMBING;
	SetAnimation((int)PlayerAnim::CLIMBING_TOP);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
}
void Player::ChangeAnimRight()
{
	look = Look::RIGHT;
	switch (state)
	{
		case State::IDLE:			SetAnimation((int)PlayerAnim::IDLE_RIGHT);			break; 
		case State::WALKING:		SetAnimation((int)PlayerAnim::WALKING_RIGHT);		break;
		case State::JUMPING:		SetAnimation((int)PlayerAnim::JUMPING_RIGHT);		break;
		case State::FALLING:		SetAnimation((int)PlayerAnim::FALLING_RIGHT);		break;
		case State::CROUCHING:		SetAnimation((int)PlayerAnim::CROUCHING_RIGHT);		break;
		case State::WHIP:			SetAnimation((int)PlayerAnim::ATTACKING_RIGHT);		break;
		case State::CROUCH_WHIP:	SetAnimation((int)PlayerAnim::CROUCH_ATTACK_RIGHT);	break;
		case State::THROWING:		SetAnimation((int)PlayerAnim::ATTACKING_RIGHT);		break;
		case State::CROUCH_THROWING:SetAnimation((int)PlayerAnim::CROUCH_ATTACK_RIGHT);	break;
	}
}
void Player::ChangeAnimLeft()
{
	look = Look::LEFT;
	switch (state)
	{
		case State::IDLE:			SetAnimation((int)PlayerAnim::IDLE_LEFT);			break; 
		case State::WALKING:		SetAnimation((int)PlayerAnim::WALKING_LEFT);		break;
		case State::JUMPING:		SetAnimation((int)PlayerAnim::JUMPING_LEFT);		break;
		case State::FALLING:		SetAnimation((int)PlayerAnim::FALLING_LEFT);		break;
		case State::CROUCHING:		SetAnimation((int)PlayerAnim::CROUCHING_LEFT);		break;
		case State::WHIP:			SetAnimation((int)PlayerAnim::ATTACKING_LEFT);		break;
		case State::CROUCH_WHIP:	SetAnimation((int)PlayerAnim::CROUCH_ATTACK_LEFT);	break;
		case State::THROWING:		SetAnimation((int)PlayerAnim::ATTACKING_LEFT);		break;
		case State::CROUCH_THROWING:SetAnimation((int)PlayerAnim::CROUCH_ATTACK_LEFT);	break;
	}
}
void Player::Update()
{
	//Player doesn't use the "Entity::Update() { pos += dir; }" default behaviour.
	//Instead, uses an independent behaviour for each axis.
	MoveX();
	MoveY();
	Static();
	weapon->Update(pos, (state == State::CROUCH_THROWING || state == State::CROUCH_WHIP));

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void Player::MoveX()
{
	AABB box;
	int prev_x = pos.x;

	//We can only go up and down while climbing
	if (state == State::CLIMBING)		return;

	//Same with crouching
	else if (state == State::CROUCHING)	return;

	else if (state == State::WHIP)	return;

	else if (state == State::CROUCH_WHIP) return;

	else if (state == State::CROUCH_THROWING) return;

	else if (state == State::THROWING)	return;

	else if (state == State::DYING)	return;

	if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT))
	{
		pos.x += -PLAYER_SPEED;
		if (state == State::IDLE) StartWalkingLeft();
		else
		{
			if (IsLookingRight()) ChangeAnimLeft();
		}

		box = GetHitbox();
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
		}
	}
	else if (IsKeyDown(KEY_RIGHT))
	{
		pos.x += PLAYER_SPEED;
		if (state == State::IDLE) StartWalkingRight();
		else
		{
			if (IsLookingLeft()) ChangeAnimRight();
		}

		box = GetHitbox();
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
		}
	}
	else
	{
		if (state == State::WALKING) Stop();
	}
}
void Player::MoveY()
{
	AABB box;
	
	// you can't move while crouching
	if (state == State::CROUCHING)	return;

	else if (state == State::WHIP)	return;

	else if (state == State::CROUCH_WHIP) return;

	else if (state == State::THROWING)	return;

	else if (state == State::CROUCH_THROWING)	return;

	else if (state == State::DYING)	return;

	else if (state == State::JUMPING)
	{
		LogicJumping();
	}
	else if (state == State::CLIMBING)
	{
		LogicClimbing();
	}
	else //idle, walking, falling
	{
		pos.y += PLAYER_SPEED;
		box = GetHitbox();
		if (map->TestCollisionGround(box, &pos.y))
		{
			if (state == State::FALLING) Stop();

			if (IsKeyDown(KEY_UP))
			{
				StartJumping();
			}
		}
		else
		{
			if (state != State::FALLING) StartFalling();
		}
	}
}
void Player::Static()
{
	AABB box;

	if (state == State::CROUCHING)
	{
		LogicCrouching();
	}
	else if (state == State::WHIP)
	{
		LogicAttack();
	}
	else if (state == State::CROUCH_WHIP)
	{
		LogicAttack();
	}
	else if (state == State::THROWING)
	{
		LogicThrow();
	}
	else if (state == State::CROUCH_THROWING)
	{
		LogicThrow();
	}
	else if (state == State::DYING)
	{
		Die();
	}
	else {
		pos.y += PLAYER_SPEED;
		box = GetHitbox();
		if (map->TestCollisionGround(box, &pos.y))
		{
			if (IsKeyDown(KEY_DOWN))
			{
				StartCrouching();
			}
			else if (IsKeyPressed(KEY_SPACE) && !IsKeyDown(KEY_DOWN))
			{
				StartWhip();
			}
			else if (IsKeyPressed(KEY_SPACE) && IsKeyDown(KEY_DOWN))
			{
				StartCrouchWhip();
			}
			else if (IsKeyPressed(KEY_Z) && !IsKeyDown(KEY_DOWN))
			{
				StartThrowing();
			}
			else if (IsKeyPressed(KEY_Z) && IsKeyDown(KEY_DOWN))
			{
				StartCrouchThrowing();
			}
			else if (IsKeyPressed(KEY_F4))
			{
				if (!GodMode) {
					StartDying();
				}
			}
			
		}
	}
}
void Player::LogicJumping()
{
	AABB box, prev_box;
	int prev_y;

	jump_delay--;
	if (jump_delay == 0)
	{
		prev_y = pos.y;
		prev_box = GetHitbox();

		pos.y += dir.y;
		dir.y += GRAVITY_FORCE;
		jump_delay = PLAYER_JUMP_DELAY;

		//Is the jump finished?
		if (dir.y > PLAYER_JUMP_FORCE)
		{
			dir.y = PLAYER_SPEED;
			StartFalling();
		}
		else
		{
			//Jumping is represented with 3 different states
			if (IsAscending())
			{
				if (IsLookingRight())	SetAnimation((int)PlayerAnim::JUMPING_RIGHT);
				else					SetAnimation((int)PlayerAnim::JUMPING_LEFT);
			}
			else if (IsLevitating())
			{
				if (IsLookingRight())	SetAnimation((int)PlayerAnim::LEVITATING_RIGHT);
				else					SetAnimation((int)PlayerAnim::LEVITATING_LEFT);
			}
			else if (IsDescending())
			{
				if (IsLookingRight())	SetAnimation((int)PlayerAnim::FALLING_RIGHT);
				else					SetAnimation((int)PlayerAnim::FALLING_LEFT);
			}
		}
		//We check ground collision when jumping down
		if (dir.y >= 0)
		{
			box = GetHitbox();

			//A ground collision occurs if we were not in a collision state previously.
			//This prevents scenarios where, after levitating due to a previous jump, we found
			//ourselves inside a tile, and the entity would otherwise be placed above the tile,
			//crossing it.
			if (!map->TestCollisionGround(prev_box, &prev_y) &&
				map->TestCollisionGround(box, &pos.y))
			{
				Stop();
			}
		}
	}
}
void Player::LogicClimbing()
{
	AABB box;
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	int tmp;

	if (IsKeyDown(KEY_UP))
	{
		pos.y -= PLAYER_LADDER_SPEED;
		sprite->NextFrame();
	}
	else if (IsKeyDown(KEY_DOWN))
	{
		pos.y += PLAYER_LADDER_SPEED;
		sprite->PrevFrame();
	}

	//It is important to first check LadderTop due to its condition as a collision ground.
	//By doing so, we ensure that we don't stop climbing down immediately after starting the descent.
	box = GetHitbox();
	if (map->TestCollisionGround(box, &pos.y))
	{
		//Case leaving the ladder descending.
		Stop();
		sprite->SetAutomaticMode();
	}
	else
	{
		if (GetAnimation() != PlayerAnim::CLIMBING)	SetAnimation((int)PlayerAnim::CLIMBING);
	}
}
void Player::LogicCrouching() 
{
	height = PLAYER_PHYSICAL_CROUCHING_HEIGHT;
	if (IsKeyPressed(KEY_SPACE)) {
		StartCrouchWhip();
	}
	else if (IsKeyPressed(KEY_Z)) {
		StartCrouchThrowing();
	}
	if (IsKeyReleased(KEY_DOWN)) {
		Stop();
		height = PLAYER_PHYSICAL_HEIGHT;
	}
}
void Player::LogicThrow()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);

	attack_delay--;
	if (attack_delay == 0)
	{
		AnimationFrame++;
		sprite->NextFrame();
		attack_delay = PLAYER_ATTACK_DELAY;

		if (AnimationFrame == 3) {
			
			if (state == State::CROUCH_THROWING) {
				StartCrouching();
			}
			else {
				Stop();
			}

			sprite->SetAutomaticMode();
			AnimationFrame = 0;
		}
		else {
			sprite->NextFrame();
		}
	}
}
void Player::LogicAttack()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);

	attack_delay--;
	if (attack_delay == 0)
	{
		AnimationFrame++;
		if (IsLookingRight()) {
			weapon->Attack(AnimationFrame, (LookAt)Look::RIGHT);
		}
		else if (IsLookingLeft()) {
			weapon->Attack(AnimationFrame, (LookAt)Look::LEFT);
		}
		sprite->NextFrame();
		attack_delay = PLAYER_ATTACK_DELAY;

		if (AnimationFrame == 3) {

			if (state == State::CROUCH_WHIP) {
				StartCrouching();
			}
			else {
				Stop();
			}

			sprite->SetAutomaticMode();
			AnimationFrame = 0;
		}
		else {
			sprite->NextFrame();
		}
	}
}
void Player::Die() 
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);

	die_delay--;
	if (die_delay == 0)
	{
		AnimationFrame++;
		sprite->NextFrame();
		die_delay = PLAYER_DYING_DELAY;

		if (AnimationFrame == 3) {

			if (state == State::DYING) {
				state = State::DEAD;
			}

			sprite->SetAutomaticMode();
			AnimationFrame = 0;
		}
		else {
			sprite->NextFrame();
		}
	}
}

void Player::DrawDebug(const Color& col) const
{	
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
	
	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 170, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}
void Player::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PLAYER);

	render->Release();
}
