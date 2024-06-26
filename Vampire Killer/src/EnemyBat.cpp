
#include "EnemyBat.h"
#include "EnemyManager.h"


EnemyBat::EnemyBat(Point pos) : Enemy({ pos.x, pos.y }, BAT_HITBOX_HEIGHT, BAT_HITBOX_WIDTH, BAT_SPRITE_HEIGHT, BAT_SPRITE_WIDTH)
{
	state = EnemyState::ADVANCING;
	look = EnemyLook::RIGHT;
	int AnimationFrame = 0;
	Damage = BAT_DAMAGE;
	type = EnemyType::BAT;

	Initialise();
}
EnemyBat::~EnemyBat()
{
}
AppStatus EnemyBat::Initialise()
{
	int i;
	const float n = (float)BAT_SPRITE_HEIGHT;
	const float n2 = (float)BAT_SPRITE_WIDTH;
	AnimationFrame = 0;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_BAT, "images/Spritesheets/Enemies & Characters/Enemy 3 Sprite Sheet.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(Resource::IMG_BAT));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)EnemyAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)EnemyAnim::ADVANCING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)EnemyAnim::ADVANCING_RIGHT, { (float)i * n, n, -n, n });
	sprite->SetAnimationDelay((int)EnemyAnim::ADVANCING_LEFT, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)EnemyAnim::ADVANCING_LEFT, { (float)i * n, n, n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::IDLE_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::IDLE_LEFT, { 0, 0, n, n });
	sprite->SetAnimationDelay((int)EnemyAnim::IDLE_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::IDLE_RIGHT, { 0, 0, -n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::EMPTY, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::EMPTY, { 0, 0, 0, 0 });

	if (EnemyManager::Instance().target->GetPos().x < 208 && EnemyManager::Instance().target->IsLookingRight()) {
		SetPos({ 255, pos.y });
		state = EnemyState::ADVANCING;
		look = EnemyLook::LEFT;
		SetAnimation((int)EnemyAnim::ADVANCING_LEFT);
		Init_pos_y = pos.y;
	}
	else if (EnemyManager::Instance().target->GetPos().x > 68 && EnemyManager::Instance().target->IsLookingLeft()) {
		SetPos({ 20, pos.y });
		state = EnemyState::ADVANCING;
		look = EnemyLook::RIGHT;
		SetAnimation((int)EnemyAnim::ADVANCING_RIGHT);
		Init_pos_y = pos.y;
	}
	else {
		isActive = false;
	}

	return AppStatus::OK;
}
void EnemyBat::Update()
{
	if (killed) {
		isActive = false;
		EnemyManager::Instance().target->IncrScore(100);
	}
	else {
		Brain();
		Sprite* sprite = dynamic_cast<Sprite*>(render);
		sprite->Update();
	}
}
void EnemyBat::Render()
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
void EnemyBat::Reset()
{
}
void EnemyBat::Brain()
{
	internalTimer += GetFrameTime();
	Move();
	if (EnemyManager::Instance().target->GetState() == State::JUMPING || EnemyManager::Instance().target->GetState() == State::FALLING) {
		AABB PlayerHitbox = EnemyManager::Instance().target->GetHitbox();
		PlayerHitbox.pos.y = EnemyManager::Instance().target->GetHitbox().pos.y - 16;
		if (this->GetHitbox().TestAABB(PlayerHitbox)) {
			DamagePlayer();
		}
	}
	else if (this->GetHitbox().TestAABB(EnemyManager::Instance().target->GetHitbox())) {
		DamagePlayer();
	}
	if (EnemyManager::Instance().target->weapon->GetFrame() == 2) {
		if (this->GetHitbox().TestAABB(EnemyManager::Instance().target->weapon->HitboxOnAttack())) {
			AudioPlayer::Instance().PlaySoundByName("Attack");
			killed = true;
		}
	}
}
void EnemyBat::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
void EnemyBat::AdvanceRight()
{
	state = EnemyState::ADVANCING;
	look = EnemyLook::RIGHT;
	SetAnimation((int)EnemyAnim::ADVANCING_RIGHT);
}
void EnemyBat::AdvanceLeft()
{
	state = EnemyState::ADVANCING;
	look = EnemyLook::LEFT;
	SetAnimation((int)EnemyAnim::ADVANCING_LEFT);
}
void EnemyBat::Move()
{
	if (state == EnemyState::IDLE) {
		if (look == EnemyLook::RIGHT) {
			AdvanceRight();
		}
		else {
			AdvanceLeft();
		}
	}
	else if (state == EnemyState::ADVANCING) {

		currentAmplitude = amplitude * sin(internalTimer * amplitudeChangeSpeed);

		pos.y = Init_pos_y + currentAmplitude * TILE_SIZE;

		if (look == EnemyLook::RIGHT) {
			pos.x += BAT_SPEED;
		}
		else {
			pos.x -= BAT_SPEED;
		}
	}
	else if (state == EnemyState::DEAD) {
		SetAnimation((int)EnemyAnim::EMPTY);
	}
}
void EnemyBat::DrawDebug(const Color& col) const
{
	DrawHitbox(BLUE);
}
void EnemyBat::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_BAT);

	render->Release();
}

