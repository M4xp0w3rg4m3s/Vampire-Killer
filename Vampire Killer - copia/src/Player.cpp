
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
	damaged_delay = 0;
	map = nullptr;
	weapon = new Weapon(p);
	score = 0;
	lives = PLAYER_INIT_LIVES;
	life = PLAYER_MAX_LIFE;
	AnimationFrame = 0;
	GodMode = false;
	isGUIinit = false;
	damaged_finished = true;
	staticJump = false;

	shield = false;
	doorKey = false;
	shield = false;
}
Player::~Player()
{
	delete weapon;
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

	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_RIGHT_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_RIGHT_SHIELD, { n*4, 0, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_LEFT_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_LEFT_SHIELD, { n*4, 0, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_RIGHT, { (float)i*n, 0, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_LEFT, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_LEFT, { (float)i*n, 0, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_RIGHT_SHIELD, ANIM_DELAY);
	for (i = 4; i < 8; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_RIGHT_SHIELD, { (float)i * n, 0, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_LEFT_SHIELD, ANIM_DELAY);
	for (i = 4; i < 8; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_LEFT_SHIELD, { (float)i * n, 0, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_RIGHT, { n, n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_LEFT, { n, n, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_RIGHT_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_RIGHT_SHIELD, { n*5, n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_LEFT_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_LEFT_SHIELD, { n*5, n, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_RIGHT, { n, n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_LEFT, { n, n, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_RIGHT_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_RIGHT_SHIELD, { n*5, n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_LEFT_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_LEFT_SHIELD, { n*5, n, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::LEVITATING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::LEVITATING_RIGHT, { n, n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::LEVITATING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::LEVITATING_LEFT, { n, n, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::LEVITATING_RIGHT_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::LEVITATING_RIGHT_SHIELD, { n*5, n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::LEVITATING_LEFT_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::LEVITATING_LEFT_SHIELD, { n*5, n, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_RIGHT, ANIM_LADDER_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_RIGHT, { (float)i * n, 2 * n, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_LEFT, ANIM_LADDER_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_LEFT, { (float)i * n, 2 * n, -n, n });
	
	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_SHIELD_RIGHT, ANIM_LADDER_DELAY);
	for (i = 4; i < 6; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_SHIELD_RIGHT, { (float)i * n, 2 * n, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_SHIELD_LEFT, ANIM_LADDER_DELAY);
	for (i = 4; i < 6; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_SHIELD_LEFT, { (float)i * n, 2 * n, -n, n });



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

	sprite->SetAnimationDelay((int)PlayerAnim::CROUCHING_RIGHT_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::CROUCHING_RIGHT_SHIELD, { n*4, n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::CROUCHING_LEFT_SHIELD, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::CROUCHING_LEFT_SHIELD, { n*4, n, -n, n });

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

	sprite->SetAnimationDelay((int)PlayerAnim::DAMAGED_RIGHT, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::DAMAGED_RIGHT, { (float)i * n, 4 * n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::DAMAGED_LEFT, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::DAMAGED_LEFT, { (float)i * n, 4 * n, -n, n });

	sprite->SetAnimation((int)PlayerAnim::IDLE_RIGHT);

	AudioPlayer::Instance().CreateSound("audio/SFX/26.wav", "MissAttack");

	AudioPlayer::Instance().CreateSound("audio/SFX/04.wav", "PlayerDamaged");

	return AppStatus::OK;
}
void Player::InitGUI() {
	InitScore();
	InitLives();
	InitLife();
	InitHearts();
	isGUIinit = true;
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
void Player::InitLives()
{
	lives = PLAYER_INIT_LIVES;
}
void Player::IncrLives(int n)
{
	lives += n;
}
void Player::DecrLives(int n)
{
	lives -= n;
}
int Player::GetLives() const
{
	return lives;
}
void Player::InitHearts()
{
	hearts = 0;
}
void Player::IncrHearts(int n)
{
	if (hearts + n >= 100) {
		IncrLives(1);
		hearts += n - 100;
	}
	else {
		hearts += n;
	}
}
void Player::DecrHearts(int n)
{
	if (hearts > 0) {
		hearts -= n;
	}
}
int Player::GetHearts() const
{
	return hearts;
}
void Player::InitLife()
{
	life = PLAYER_MAX_LIFE;
}
void Player::IncrLife(int n)
{
	if (life <= 16) {
		life += n;
	}
}
void Player::DecrLife(int n)
{
	if (GodMode) return;
	
	if (damaged_delay > 0) return;

	if (!shield) {
		if (life - n > 0) {
			life -= n;
			damaged_delay = PLAYER_DAMAGED_DELAY;
			AudioPlayer::Instance().PlaySoundByName("PlayerDamaged");
			StartDamaged();
		}
		else if (life - n <= 0){
			AudioPlayer::Instance().PlaySoundByName("PlayerDamaged");
			StartDying();
		}
	}
	else {
		if (life - (n/2) > 0) {
			life -= (n/2);
			damaged_delay = PLAYER_DAMAGED_DELAY;
			AudioPlayer::Instance().PlaySoundByName("PlayerDamaged");
			StartDamaged();
		}
		else if (life - (n/2) <= 0) {
			AudioPlayer::Instance().PlaySoundByName("PlayerDamaged");
			StartDying();
		}
	}
}
int Player::GetLife() const
{
	return life;
}
int Player::GetDamagedDelay() const
{
	return damaged_delay;
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
bool Player::HasChestKey() const {
	return chestKey;
}
void Player::SwitchChestKey()
{
	if (HasChestKey()) {
		chestKey = false;
	}
	else {
		chestKey = true;
	}
}
bool Player::HasDoorKey() const
{
	return doorKey;
}
void Player::SwitchDoorKey()
{
	if (HasDoorKey()) {
		doorKey = false;
	}
	else {
		doorKey = true;
	}
}
bool Player::HasShield() const
{
	return shield;
}
void Player::SwitchShield()
{
	if (HasShield()) {
		shield = false;
	}
	else {
		shield = true;
	}
}
void Player::GodModeSwitch() {
	if (GodMode == true) {
		GodMode = false;
	}
	else {
		GodMode = true;
	}
}
void Player::SetState(State givenState)
{
	state = givenState;
}
void Player::SetLook(Look givenLook)
{
	look = givenLook;
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
	if (!shield) {
		dir = { 0,0 };
		state = State::IDLE;
		if (IsLookingRight())	SetAnimation((int)PlayerAnim::IDLE_RIGHT);
		else					SetAnimation((int)PlayerAnim::IDLE_LEFT);
	}
	else {
		dir = { 0,0 };
		state = State::IDLE;
		if (IsLookingRight())	SetAnimation((int)PlayerAnim::IDLE_RIGHT_SHIELD);
		else					SetAnimation((int)PlayerAnim::IDLE_LEFT_SHIELD);
	}
}
void Player::StartWalkingLeft()
{
	if (!shield) {
		state = State::WALKING;
		look = Look::LEFT;
		SetAnimation((int)PlayerAnim::WALKING_LEFT);
	}
	else {
		state = State::WALKING;
		look = Look::LEFT;
		SetAnimation((int)PlayerAnim::WALKING_LEFT_SHIELD);
	}
}
void Player::StartWalkingRight()
{
	if (!shield) {
		state = State::WALKING;
		look = Look::RIGHT;
		SetAnimation((int)PlayerAnim::WALKING_RIGHT);
	}
	else {
		state = State::WALKING;
		look = Look::RIGHT;
		SetAnimation((int)PlayerAnim::WALKING_RIGHT_SHIELD);
	}
}
void Player::StartFalling()
{
	if (!shield) {
		dir.y = PLAYER_SPEED;
		state = State::FALLING;
		if (IsLookingRight())	SetAnimation((int)PlayerAnim::FALLING_RIGHT);
		else					SetAnimation((int)PlayerAnim::FALLING_LEFT);
	}
	else {
		dir.y = PLAYER_SPEED;
		state = State::FALLING;
		if (IsLookingRight())	SetAnimation((int)PlayerAnim::FALLING_RIGHT_SHIELD);
		else					SetAnimation((int)PlayerAnim::FALLING_LEFT_SHIELD);
	}
}
void Player::StartJumping()
{
	if (!damaged_finished) return;
	if (!shield) {
		dir.y = -PLAYER_JUMP_FORCE;
		state = State::JUMPING;
		if (IsLookingRight())	SetAnimation((int)PlayerAnim::JUMPING_RIGHT);
		else					SetAnimation((int)PlayerAnim::JUMPING_LEFT);
		jump_delay = PLAYER_JUMP_DELAY;
	}
	else {
		dir.y = -PLAYER_JUMP_FORCE;
		state = State::JUMPING;
		if (IsLookingRight())	SetAnimation((int)PlayerAnim::JUMPING_RIGHT_SHIELD);
		else					SetAnimation((int)PlayerAnim::JUMPING_LEFT_SHIELD);
		jump_delay = PLAYER_JUMP_DELAY;
	}
}
void Player::StartWhip() {
	state = State::WHIP;
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
	if (!shield) {
		state = State::CROUCHING;
		if (IsLookingRight())	SetAnimation((int)PlayerAnim::CROUCHING_RIGHT);
		else					SetAnimation((int)PlayerAnim::CROUCHING_LEFT);
	}
	else {
		state = State::CROUCHING;
		if (IsLookingRight())	SetAnimation((int)PlayerAnim::CROUCHING_RIGHT_SHIELD);
		else					SetAnimation((int)PlayerAnim::CROUCHING_LEFT_SHIELD);
	}
}
void Player::StartCrouchWhip() {
	state = State::CROUCH_WHIP;
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
void Player::StartDamaged()
{
	if (state == State::JUMPING || state == State::FALLING || state == State::WHIP) {
		Stop();
		weapon->Attack(-1, LookAt::RIGHT);
	}
	state = State::DAMAGED;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::DAMAGED_RIGHT);
	else					SetAnimation((int)PlayerAnim::DAMAGED_LEFT);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
	die_delay = PLAYER_DYING_DELAY;
	damaged_finished = false;
}
void Player::StartClimbingUp()
{
	state = State::CLIMBING;
	if (!shield) 
	{
		if (IsLookingRight())
		{
			SetAnimation((int)PlayerAnim::CLIMBING_RIGHT);
		}
		else
		{
			SetAnimation((int)PlayerAnim::CLIMBING_LEFT);
		}
	}
	else 
	{
		if (IsLookingRight())
		{
			SetAnimation((int)PlayerAnim::CLIMBING_SHIELD_RIGHT);
		}
		else
		{
			SetAnimation((int)PlayerAnim::CLIMBING_SHIELD_LEFT);
		}
	}
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
}
void Player::StartClimbingDown()
{
	state = State::CLIMBING;

	if (!shield)
	{
		if (IsLookingRight())
		{
			SetAnimation((int)PlayerAnim::CLIMBING_RIGHT);
		}
		else
		{
			SetAnimation((int)PlayerAnim::CLIMBING_LEFT);
		}
	}
	else
	{
		if (IsLookingRight())
		{
			SetAnimation((int)PlayerAnim::CLIMBING_SHIELD_RIGHT);
		}
		else
		{
			SetAnimation((int)PlayerAnim::CLIMBING_SHIELD_LEFT);
		}
	}
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
}
void Player::ChangeAnimRight()
{
	look = Look::RIGHT;
	if (!shield) {
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
	else {
		switch (state)
		{
		case State::IDLE:			SetAnimation((int)PlayerAnim::IDLE_RIGHT_SHIELD);			break;
		case State::WALKING:		SetAnimation((int)PlayerAnim::WALKING_RIGHT_SHIELD);		break;
		case State::JUMPING:		SetAnimation((int)PlayerAnim::JUMPING_RIGHT_SHIELD);		break;
		case State::FALLING:		SetAnimation((int)PlayerAnim::FALLING_RIGHT_SHIELD);		break;
		case State::CROUCHING:		SetAnimation((int)PlayerAnim::CROUCHING_RIGHT_SHIELD);		break;
		case State::WHIP:			SetAnimation((int)PlayerAnim::ATTACKING_RIGHT);		break;
		case State::CROUCH_WHIP:	SetAnimation((int)PlayerAnim::CROUCH_ATTACK_RIGHT);	break;
		case State::THROWING:		SetAnimation((int)PlayerAnim::ATTACKING_RIGHT);		break;
		case State::CROUCH_THROWING:SetAnimation((int)PlayerAnim::CROUCH_ATTACK_RIGHT);	break;
		}
	}
}
void Player::ChangeAnimLeft()
{
	look = Look::LEFT;
	if (!shield) {
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
	else {
		switch (state)
		{
		case State::IDLE:			SetAnimation((int)PlayerAnim::IDLE_LEFT_SHIELD);			break;
		case State::WALKING:		SetAnimation((int)PlayerAnim::WALKING_LEFT_SHIELD);		break;
		case State::JUMPING:		SetAnimation((int)PlayerAnim::JUMPING_LEFT_SHIELD);		break;
		case State::FALLING:		SetAnimation((int)PlayerAnim::FALLING_LEFT_SHIELD);		break;
		case State::CROUCHING:		SetAnimation((int)PlayerAnim::CROUCHING_LEFT_SHIELD);		break;
		case State::WHIP:			SetAnimation((int)PlayerAnim::ATTACKING_LEFT);		break;
		case State::CROUCH_WHIP:	SetAnimation((int)PlayerAnim::CROUCH_ATTACK_LEFT);	break;
		case State::THROWING:		SetAnimation((int)PlayerAnim::ATTACKING_LEFT);		break;
		case State::CROUCH_THROWING:SetAnimation((int)PlayerAnim::CROUCH_ATTACK_LEFT);	break;
		}
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
	damaged_delay--;

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void Player::MoveX()
{
	AABB box;
	int prev_x = pos.x;

	if (state == State::CROUCHING || state == State::CROUCH_WHIP ||
		state == State::CROUCH_THROWING || state == State::DYING)
		return;

	if (staticJump) return;

	if (state == State::IDLE && IsKeyPressed(KEY_UP)) {
		staticJump = true;
		return;
	}
	if (state != State::DAMAGED) {
		if (state == State::WHIP || state == State::THROWING) {
			box = GetHitbox();
			if (!map->TestCollisionGround(box, &pos.y))
			{
				/*if (look == Look::LEFT) {
					pos.x += -PLAYER_SPEED;
				}
				else {
					pos.x += PLAYER_SPEED;
				}*/
			}
		}
		else if (state == State::JUMPING && (state != State::WHIP && state != State::THROWING)) {
			if (look == Look::LEFT) {
				pos.x += -PLAYER_SPEED;
			}
			else {
				pos.x += PLAYER_SPEED;
			}
		}
		else if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT))
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
		else if (IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT))
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
}
void Player::MoveY()
{
	AABB box;
	
	if (state == State::CROUCHING || state == State::CROUCH_WHIP || state == State::CROUCH_THROWING || state == State::DYING)	return;

	if (state != State::DAMAGED) {
		if (state == State::JUMPING)
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

				if (state != State::WHIP && state != State::THROWING) {
					if (IsKeyDown(KEY_UP))
					{
						StartJumping();

						if (map->TestCollisionStairs(box) && IsKeyPressed(KEY_UP))
						{
							StartClimbingUp();
						}
						else if (map->TestCollisionStairs(box) && IsKeyPressed(KEY_DOWN))
						{
							StartClimbingDown();
						}
					}
				}
			}
			else
			{
				if (state != State::WHIP && state != State::THROWING) {
					if (state != State::FALLING) StartFalling();
				}
			}
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
	else if (state == State::WHIP || state == State::CROUCH_WHIP)
	{
		LogicAttack();
	}
	else if (state == State::THROWING || state == State::CROUCH_THROWING)
	{
		LogicThrow();
	}
	else if (state == State::DYING)
	{
		Die();
	}
	else if (state == State::DAMAGED) {
		LogicDamaged();
	}
	else {
		pos.y += PLAYER_SPEED;
		box = GetHitbox();
		if (IsKeyPressed(KEY_SPACE))
		{
			StartWhip();
		}
		else if (IsKeyPressed(KEY_Z))
		{
			StartThrowing();
		}
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
			if (!shield) {
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
			else if (shield) {
				if (IsAscending())
				{
					if (IsLookingRight())	SetAnimation((int)PlayerAnim::JUMPING_RIGHT_SHIELD);
					else					SetAnimation((int)PlayerAnim::JUMPING_LEFT_SHIELD);
				}
				else if (IsLevitating())
				{
					if (IsLookingRight())	SetAnimation((int)PlayerAnim::LEVITATING_RIGHT_SHIELD);
					else					SetAnimation((int)PlayerAnim::LEVITATING_LEFT_SHIELD);
				}
				else if (IsDescending())
				{
					if (IsLookingRight())	SetAnimation((int)PlayerAnim::FALLING_RIGHT_SHIELD);
					else					SetAnimation((int)PlayerAnim::FALLING_LEFT_SHIELD);
				}
			}
			//Jumping is represented with 3 different states
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
				staticJump = false;
				height = PLAYER_PHYSICAL_HEIGHT;
			}
		}
	}
}
void Player::LogicClimbing()
{
	AABB box;
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	int tmp;
	if (!map->TestCollisionStairs(GetHitbox()))
	{
		Stop();
	}
	if (IsKeyDown(KEY_UP) && IsLookingLeft())
	{
		pos.y -= 3;
		pos.x -= PLAYER_LADDER_SPEED;
		sprite->NextFrame();
	}
	else if (IsKeyDown(KEY_DOWN) && IsKeyDown(KEY_LEFT))
	{
		pos.y += PLAYER_LADDER_SPEED;
		pos.x -= PLAYER_LADDER_SPEED;
		sprite->PrevFrame();
	}

	if (IsKeyDown(KEY_UP) && IsLookingRight())
	{
		pos.y -= 3;
		pos.x += PLAYER_LADDER_SPEED;
		sprite->NextFrame();
	}
	else if (IsKeyDown(KEY_DOWN) && IsKeyDown(KEY_RIGHT))
	{
		pos.y += PLAYER_LADDER_SPEED;
		pos.x += PLAYER_LADDER_SPEED;
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
		if (!shield)
		{
			if (IsLookingRight())
			{
				if (GetAnimation() != PlayerAnim::CLIMBING_RIGHT)	SetAnimation((int)PlayerAnim::CLIMBING_RIGHT);
			}
			else
			{
				if (GetAnimation() != PlayerAnim::CLIMBING_LEFT)	SetAnimation((int)PlayerAnim::CLIMBING_LEFT);
			}
		}
		else
		{
			if (IsLookingRight())
			{
				if (GetAnimation() != PlayerAnim::CLIMBING_SHIELD_RIGHT)	SetAnimation((int)PlayerAnim::CLIMBING_SHIELD_RIGHT);
			}
			else
			{
				if (GetAnimation() != PlayerAnim::CLIMBING_SHIELD_LEFT)	SetAnimation((int)PlayerAnim::CLIMBING_SHIELD_LEFT);
			}
		}
		
		
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
			
			if (state == State::CROUCH_THROWING && IsKeyDown(KEY_DOWN)) {
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

			if (state == State::CROUCH_WHIP && IsKeyDown(KEY_DOWN)) {
				StartCrouching();
			}
			else {
				Stop();
				staticJump = false;
			}

			AudioPlayer::Instance().PlaySoundByName("MissAttack");

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

		if (AnimationFrame <= 3) {

			if (state == State::DYING) {
				state = State::DEAD;
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
void Player::LogicDamaged()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);

	die_delay--;
	if (die_delay > PLAYER_DYING_DELAY / 2) {
		if (look == Look::RIGHT) {
			pos.x -= PLAYER_SPEED;
		}
		else {
			pos.x += PLAYER_SPEED;
		}
		pos.y -= PLAYER_SPEED;
	}
	if (die_delay <= 0)
	{
		AnimationFrame++;
		sprite->NextFrame();
		die_delay = PLAYER_DYING_DELAY;

		if (AnimationFrame >= 2) {
			Stop();
			staticJump = false;
			damaged_finished = true;

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
	if (!IsGodMode()) {
		Entity::DrawHitbox(pos.x, pos.y, width, height, col);
	}
	else {
		Entity::DrawHitbox(pos.x, pos.y, width, height, PINK);
	}
	
	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 170, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}
void Player::Release()
{
	weapon->Release();
	render->Release();
}
State Player::GetState() const
{
	return state;
}
