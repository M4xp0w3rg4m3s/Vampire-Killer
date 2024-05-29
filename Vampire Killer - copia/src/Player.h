#pragma once
#include "Entity.h"
#include "Weapon.h"
#include "TileMap.h"
#include "AudioPlayer.h"

//Representation model size: 32x32
#define PLAYER_FRAME_SIZE	32

//Logical model size: 12x28
#define PLAYER_PHYSICAL_WIDTH	12
#define PLAYER_PHYSICAL_HEIGHT	28

#define PLAYER_PHYSICAL_CROUCHING_HEIGHT	22

//The maximum amount of lives a player can have and the initial lives
#define PLAYER_MAX_LIVES		2
#define PLAYER_MAX_LIFE			16

//Horizontal speed and vertical speed while falling down
#define PLAYER_SPEED			1

//Vertical speed while on a ladder
#define PLAYER_LADDER_SPEED		1

//Frame animation delay while on a ladder
#define ANIM_LADDER_DELAY		(2*ANIM_DELAY)

//When jumping, initial jump speed and maximum falling speed
#define PLAYER_JUMP_FORCE		10

//Frame delay for updating the jump velocity
#define PLAYER_JUMP_DELAY		2

//Frame delay for updating the attack velocity
#define PLAYER_ATTACK_DELAY		6

//Frame delay for updating the dying velocity
#define PLAYER_DYING_DELAY		20

//Frame delay for updating the being damaged velocity
#define PLAYER_DAMAGED_DELAY	3*60

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
	DAMAGED,
	DYING, DEAD,
	WIN
};
enum class Look { RIGHT, LEFT };


//Rendering states
enum class PlayerAnim {
	IDLE_LEFT, IDLE_RIGHT,
	IDLE_LEFT_SHIELD, IDLE_RIGHT_SHIELD,
	WALKING_LEFT, WALKING_RIGHT,
	WALKING_LEFT_SHIELD, WALKING_RIGHT_SHIELD,
	JUMPING_LEFT, JUMPING_RIGHT,
	JUMPING_LEFT_SHIELD, JUMPING_RIGHT_SHIELD,
	LEVITATING_LEFT, LEVITATING_RIGHT,
	LEVITATING_LEFT_SHIELD, LEVITATING_RIGHT_SHIELD,
	FALLING_LEFT, FALLING_RIGHT,
	FALLING_LEFT_SHIELD, FALLING_RIGHT_SHIELD,
	CLIMBING, CLIMBING_PRE_TOP, CLIMBING_TOP,
	CLIMBING_SHIELD, CLIMBING_PRE_TOP_SHIELD, CLIMBING_TOP_SHIELD,
	DAMAGED_LEFT, DAMAGED_RIGHT,
	DYING_LEFT, DYING_RIGHT,
	ATTACKING_LEFT, ATTACKING_RIGHT,
	CROUCHING_LEFT, CROUCHING_RIGHT,
	CROUCHING_LEFT_SHIELD, CROUCHING_RIGHT_SHIELD,
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

	void InitGUI();
	bool isGUIinit;

	void InitScore();
	void IncrScore(int n);
	int GetScore() const;

	void InitLife();
	void IncrLife(int n);
	void DecrLife(int n);
	int GetLife() const;
	int GetDamagedDelay() const;

	void InitLives();
	void IncrLives(int n);
	void DecrLives(int n);
	int GetLives() const;

	void InitHearts();
	void IncrHearts(int n);
	int GetHearts() const;

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();
	State GetState() const;
	
	bool IsGodMode() const;
	bool IsDead() const;
	bool HasWon() const;
	void GodModeSwitch();

	void SetState(State givenState);
	void SetLook(Look givenLook);

	void Win();

	//Inventory management
	bool HasChestKey() const;
	void SwitchChestKey();
	bool HasDoorKey() const;
	void SwitchDoorKey();
	bool HasShield() const;
	void SwitchShield();

	Weapon* weapon;

	void StartDying();

	bool IsLookingRight() const;
	bool IsLookingLeft() const;
private:

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
	void LogicDamaged();
	

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
	void StartCrouchWhip();
	void StartCrouchThrowing();
	void StartClimbingUp();
	void StartClimbingDown();
	void StartDamaged();
	void ChangeAnimRight();
	void ChangeAnimLeft();

	//Jump steps
	bool IsAscending() const;
	bool IsLevitating() const;
	bool IsDescending() const;


	//Ladder get in/out steps
	bool IsInFirstHalfTile() const;
	bool IsInSecondHalfTile() const;

	bool staticJump;
	bool damaged_finished;
	bool GodMode;
	State state;
	Look look;
	int jump_delay;
	int attack_delay;
	int	damaged_delay;
	int	die_delay;
	int AnimationFrame;

	//Inventory Items
	bool chestKey;
	bool doorKey;
	bool shield;

	TileMap *map;

	//HUD Numbers
	int score;
	int lives;
	int life;
	int hearts;
};

