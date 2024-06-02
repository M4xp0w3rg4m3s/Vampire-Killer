#include "Boss.h"
#include "EnemyManager.h"

Boss::Boss(Point pos) : Enemy(pos, BOSS_HITBOX_HEIGHT, BOSS_HITBOX_WIDTH, BOSS_SPRITE_HEIGHT, BOSS_SPRITE_WIDTH)
{
	state = EnemyState::ADVANCING;
	look = EnemyLook::RIGHT;
	int AnimationFrame = 0;
	Damage = BOSS_DAMAGE;


	type = EnemyType::BOSS;

	Initialise();
}

Boss::~Boss()
{
}

AppStatus Boss::Initialise()
{
	int i;
	const float n = (float)BOSS_SPRITE_HEIGHT;
	const float n2 = (float)BOSS_SPRITE_WIDTH;
	AnimationFrame = 0;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_BOSS, "images/Spritesheets/Enemies & Characters/Boss 1 Sprite Sheet.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(Resource::IMG_BOSS));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)EnemyAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)EnemyAnim::IDLE_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::IDLE_RIGHT, { 0, 0, n2, n });

	sprite->SetAnimationDelay((int)EnemyAnim::MOVING, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)EnemyAnim::MOVING, { (float)i * n2, 0, n2, n });


	
	state = EnemyState::ADVANCING;
	SetAnimation((int)EnemyAnim::MOVING);
	Init_pos_y = pos.y;

	return AppStatus::OK;
}

void Boss::Update()
{
	if (killed) {
		isActive = false;
		EnemyManager::Instance().target->IncrScore(2000);
	}
	else {
		if (justHit) {
			hitCounter--;
		}
		if (hitCounter <= 0 || !justHit) {
			Brain();
		}
		Sprite* sprite = dynamic_cast<Sprite*>(render);
		sprite->Update();
	}
}

void Boss::Render()
{
	Point p = GetRenderingPosition();
	if (!killed) {
		render->Draw(p.x, p.y);
	}
}

void Boss::Reset()
{

}

void Boss::Brain()
{
	internalTimer += GetFrameTime();
	
	if (pos.x == 232)
	{
		Moving = false;
	}
	if (pos.x == 25)
	{
		Moving = true;
	}

	Move();

	if (EnemyManager::Instance().target->GetState() == State::JUMPING || EnemyManager::Instance().target->GetState() == State::FALLING) {
	
		if(this->GetHitbox().TestAABB(EnemyManager::Instance().target->GetHitbox())) {
			DamagePlayer();
		}
	}
	else if (this->GetHitbox().TestAABB(EnemyManager::Instance().target->GetHitbox())) {
		DamagePlayer();
	}
	if (this->GetHitbox().TestAABB(EnemyManager::Instance().target->weapon->HitboxOnAttack())) {
		AudioPlayer::Instance().PlaySoundByName("Attack");
		if (EnemyManager::Instance().target->weapon->GetWeaponType() == WeaponType::WHIP) {
			life -= 2;
		}
		else if(EnemyManager::Instance().target->weapon->GetWeaponType() == WeaponType::CHAIN) {
			life -= 3;
		}
		if (life <= 0) {
			killed = true;
			life = 0;
		}
		justHit = true;
		hitCounter = 30;
	}
}

void Boss::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}

int Boss::GetLife() const
{
	return life;
}

void Boss::Move()
{
	if (Moving)
	{
		pos.x += BOSS_SPEED;
	}
	if (!Moving)
	{
		pos.x -= BOSS_SPEED;
	}

	currentAmplitude = amplitude * sin(internalTimer * amplitudeChangeSpeed);

	pos.y = Init_pos_y + currentAmplitude * TILE_SIZE;


	//AABB box = this->GetHitbox();

	//if (!map->TestCollisionEmptyEnemies(box))
	//{
	//	if (ToCalculateVec)
	//	{
	//		vec = { (float)GetRandomValue(-1,1),(float)GetRandomValue(-3, 3) };
	//		counter = 30 * GetRandomValue(1, 2);
	//		ToCalculateVec = false;
	//		Straight_or_curve = GetRandomValue(0, 1);
	//		
	//	}
	//	if (counter >= 0 && Straight_or_curve == 1)
	//	{
	//		pos.x += vec.x;
	//		pos.y += vec.y;
	//		counter--;
	//	}
	//	/*else if (counter >= 0 && Straight_or_curve == 0)
	//	{
	//		pos.x += vec.x;

	//		currentAmplitude = amplitude * sin(internalTimer * amplitudeChangeSpeed);
	//		pos.y = Init_pos_y + currentAmplitude * TILE_SIZE;
	//			
	//		counter--;
	//	}*/
	//	else
	//	{
	//		ToCalculateVec = true;
	//	}
	//}
	//else
	//{
	//	vec.x = -vec.x;
	//	vec.y = -vec.y;

	//	/*counter = 30 * GetRandomValue(1, 3);*/

	//	ToCalculateVec = false;

	//	if (pos.x <= 256)
	//	{
	//		pos.x -= BOSS_SPEED;
	//	}
	//	else if (pos.x > 16)
	//	{
	//		pos.x += BOSS_SPEED;
	//	}

	//	if (pos.y <= 16)
	//	{
	//		pos.y += BOSS_SPEED;
	//	}
	//	else if (pos.y > 176/* && map->TestCollisionEmptyEnemies(box)*/)
	//	{
	//		pos.y -= BOSS_SPEED;
	//	}

	//	/*counter--;*/
	//}

}

void Boss::DrawDebug(const Color& col) const
{
	DrawHitbox(GRAY);
}

void Boss::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_BOSS);
	render->Release();
}


