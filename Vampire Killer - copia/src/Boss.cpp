#include "Boss.h"
#include "EnemyManager.h"

Boss::Boss(Point pos)
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
	sprite->AddKeyFrame((int)EnemyAnim::IDLE_RIGHT, { 0, 0, n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::MOVING, ANIM_DELAY);
	for (i = 0; i < 1; ++i)
		sprite->AddKeyFrame((int)EnemyAnim::MOVING, { (float)i * n, 0, n, n });



	if (EnemyManager::Instance().target->GetPos().x < 208 && EnemyManager::Instance().target->IsLookingRight()) {
		SetPos({ 255, pos.y });
		state = EnemyState::ADVANCING;
		look = EnemyLook::LEFT;
		SetAnimation((int)EnemyAnim::MOVING);
	}
	else if (EnemyManager::Instance().target->GetPos().x > 68 && EnemyManager::Instance().target->IsLookingLeft()) {
		SetPos({ 20, pos.y });
		state = EnemyState::ADVANCING;
		look = EnemyLook::RIGHT;
		SetAnimation((int)EnemyAnim::MOVING);
	}
	else {
		isActive = false;
	}
	if (EnemyManager::Instance().target->GetPos().y < 130 && EnemyManager::Instance().target->IsLookingRight()) {
		SetPos({ pos.x, 79 });
	}
	return AppStatus::OK;
}

void Boss::Update()
{
	if (killed) {
		--killed_time;
		if (killed_time < 0) {
			isActive = false;
			EnemyManager::Instance().target->IncrScore(100);
		}
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
		else 
		{
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


