#pragma once
#include "Entity.h"
#include "Weapon.h"
#include "TileMap.h"

//Representation model size: 32x32
#define PLAYER_FRAME_SIZE	32

//Logical model size: 12x28
#define PLAYER_PHYSICAL_WIDTH	12
#define PLAYER_PHYSICAL_HEIGHT	28

#define PLAYER_PHYSICAL_CROUCHING_HEIGHT	22

//Horizontal speed and vertical speed while falling down
#define PLAYER_SPEED			1

//Vertical speed while on a ladder
#define PLAYER_LADDER_SPEED		1

//Frame animation delay while on a ladder
#define ANIM_LADDER_DELAY		(2*ANIM_DELAY)

//When jumping, initial jump speed and maximum falling speed
#define PLAYER_JUMP_FORCE		8

//Frame delay for updating the jump velocity
#define PLAYER_JUMP_DELAY		2

//Frame delay for updating the attack velocity
#define PLAYER_ATTACK_DELAY		6

//Frame delay for updating the dying velocity
#define PLAYER_DYING_DELAY		20

//Player is levitating when abs(speed) <= this value
#define PLAYER_LEVITATING_SPEED	4

//Gravity affects jumping velocity when jump_delay is 0
#define GRAVITY_FORCE			1

//Logic states
enum class State {
	IDLE, WALKING, JUMPING, FALLING,
	WHIP, CROUCH_WHIP,
	THROWING, CROUCH_THROWING,
	CLIMBING, 
	CROUCHING, 
	DYING, DEAD,
	WIN
};
enum class Look { RIGHT, LEFT };


//Rendering states
enum class PlayerAnim {
	IDLE_LEFT, IDLE_RIGHT,
	WALKING_LEFT, WALKING_RIGHT,
	JUMPING_LEFT, JUMPING_RIGHT,
	LEVITATING_LEFT, LEVITATING_RIGHT,
	FALLING_LEFT, FALLING_RIGHT,
	CLIMBING, CLIMBING_PRE_TOP, CLIMBING_TOP,
	DYING_LEFT, DYING_RIGHT,
	ATTACKING_LEFT, ATTACKING_RIGHT,
	CROUCHING_LEFT, CROUCHING_RIGHT,
	CROUCH_ATTACK_LEFT, CROUCH_ATTACK_RIGHT,
	STANDING,
	NUM_ANIMATIONS
};

class Player: public Entity
{
public:
	Player(const Point& p, State s, Look view);
	~Player();
	
	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void InitScore();
	void IncrScore(int n);
	int GetScore() const;

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();
	
	bool IsGodMode() const;
	bool IsDead() const;
	bool HasWon() const;
	void GodModeSwitch();

	void Win();

	Weapon* weapon;

private:
	bool IsLookingRight() const;
	bool IsLookingLeft() const;

	//Player mechanics
	void MoveX();
	void MoveY();
	void Static();
	void LogicJumping();
	void LogicClimbing();
	void LogicCrouching();
	void LogicThrow();
	void LogicAttack();
	void Die();
	

	//Animation management
	void SetAnimation(int id);
	PlayerAnim GetAnimation() const;
	void Stop();
	void StartWalkingLeft();
	void StartWalkingRight();
	void StartFalling();
	void StartJumping(); 
	void StartWhip();
	void StartThrowing();
	void StartCrouching();
	void StartDying();
	void StartCrouchWhip();
	void StartCrouchThrowing();
	void StartClimbingUp();
	void StartClimbingDown();
	void ChangeAnimRight();
	void ChangeAnimLeft();

	//Jump steps
	bool IsAscending() const;
	bool IsLevitating() const;
	bool IsDescending() const;


	//Ladder get in/out steps
	bool IsInFirstHalfTile() const;
	bool IsInSecondHalfTile() const;

	bool GodMode;
	State state;
	Look look;
	int jump_delay;
	int attack_delay;
	int	die_delay;
	int AnimationFrame;

	TileMap *map;

	int score;
};

