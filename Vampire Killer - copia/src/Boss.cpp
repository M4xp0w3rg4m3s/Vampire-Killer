#include "Boss.h"
#include "EnemyManager.h"

Boss::Boss(Point pos) : Enemy({ pos.x, pos.y }, BOSS_HITBOX_HEIGHT, BOSS_HITBOX_WIDTH, BOSS_SPRITE_HEIGHT, BOSS_SPRITE_WIDTH)
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


	SetPos({ pos.x, pos.y });
	state = EnemyState::ADVANCING;
	look = EnemyLook::LEFT;
	SetAnimation((int)EnemyAnim::MOVING);

	return AppStatus::OK;
}

void Boss::Update()
{
	if (killed) {
		isActive = false;
		EnemyManager::Instance().target->IncrScore(2000);
	}
	else {
		Brain();
		Sprite* sprite = dynamic_cast<Sprite*>(render);
		sprite->Update();
	}
}

void Boss::Render()
{
	if (pos.x > 16 && pos.x < 256)
	{
		Point p = GetRenderingPosition();
		if (!killed) {
			render->Draw(p.x, p.y);
		}
	}
	else {
		isActive = false;
	}
}

void Boss::Reset()
{

}

void Boss::Brain()
{
	internalTimer += GetFrameTime();

	Move();

	if (EnemyManager::Instance().target->GetState() == State::JUMPING || EnemyManager::Instance().target->GetState() == State::FALLING) {
		AABB PlayerHitbox = EnemyManager::Instance().target->GetHitbox();
		PlayerHitbox.pos.y = EnemyManager::Instance().target->GetHitbox().pos.y-16;
		if(this->GetHitbox().TestAABB(PlayerHitbox)) {
			DamagePlayer();
		}
	}
	else if (this->GetHitbox().TestAABB(EnemyManager::Instance().target->GetHitbox())) {
		DamagePlayer();
	}
	if (this->GetHitbox().TestAABB(EnemyManager::Instance().target->weapon->HitboxOnAttack())) {
		AudioPlayer::Instance().PlaySoundByName("Attack");
		killed = true;
	}
}

void Boss::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}

void Boss::Move()
{
	if (ToCalculateVec) 
	{
		vec = { (float)GetRandomValue(-2,2),(float)GetRandomValue(-2, 2) };
		counter = 60 * GetRandomValue(1,3);
		ToCalculateVec = false;
		Straight_or_curve = GetRandomValue(0, 1);
		Curve_Up_or_Down = GetRandomValue(0, 1);
	}
	if (counter >= 0 && Straight_or_curve == 1)
	{
		pos.x += vec.x;
		pos.y += vec.y;
		counter--;
	}
	else if (counter >= 0 && Straight_or_curve == 0)
	{
		if (Curve_Up_or_Down == 1)
		{
			currentAmplitude = amplitude * sin(internalTimer * amplitudeChangeSpeed);
			pos.y = Init_pos_y + currentAmplitude * TILE_SIZE;
		}
		else
		{
			currentAmplitude = amplitude * sin(internalTimer * amplitudeChangeSpeed);
			pos.y = Init_pos_y + currentAmplitude * TILE_SIZE * (- 1);
		}
	}
	else 
	{
		ToCalculateVec = true;
	}
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


