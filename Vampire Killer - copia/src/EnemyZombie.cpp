
#include "EnemyZombie.h"
#include "EnemyManager.h"

EnemyZombie::EnemyZombie(Point pos) : Enemy(pos, ZOMBIE_HITBOX_HEIGHT, ZOMBIE_HITBOX_WIDTH, ZOMBIE_SPRITE_HEIGHT, ZOMBIE_SPRITE_WIDTH)
{
	state = EnemyState::ADVANCING;
	look = EnemyLook::LEFT;
	int AnimationFrame = 0;
	Damage = ZOMBIE_DAMAGE;

	Initialise();
}
EnemyZombie::~EnemyZombie()
{
	Release();
}
AppStatus EnemyZombie::Initialise()
{
	int i;
	const float n = (float)ZOMBIE_SPRITE_HEIGHT;
	const float n2 = (float)ZOMBIE_SPRITE_WIDTH;
	AnimationFrame = 0;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_ZOMBIE, "images/Spritesheets/Enemies & Characters/Enemy 1 Sprite Sheet.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(Resource::IMG_ZOMBIE));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)EnemyAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)EnemyAnim::ADVANCING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)EnemyAnim::ADVANCING_RIGHT, { (float)i * n, 0, -n, n });
	sprite->SetAnimationDelay((int)EnemyAnim::ADVANCING_LEFT, ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)EnemyAnim::ADVANCING_LEFT, { (float)i * n, 0, n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::EMPTY, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::EMPTY, { 0, 0, 0, 0 });

	if (EnemyManager::Instance().target->GetPos().x < 208 && EnemyManager::Instance().target->IsLookingRight()) {
		SetPos({ 255, pos.y });
		state = EnemyState::ADVANCING;
		look = EnemyLook::LEFT;
		SetAnimation((int)EnemyAnim::ADVANCING_LEFT);
	}
	else if (EnemyManager::Instance().target->GetPos().x > 68 && EnemyManager::Instance().target->IsLookingLeft()) {
		SetPos({ 20, pos.y });
		state = EnemyState::ADVANCING;
		look = EnemyLook::RIGHT;
		SetAnimation((int)EnemyAnim::ADVANCING_RIGHT);
	}
	else {
		isActive = false;
	}

	return AppStatus::OK;
}
void EnemyZombie::Update()
{
	Brain();
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void EnemyZombie::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
void EnemyZombie::Render()
{
	if (pos.x > 16 && pos.x < 256)
	{
		Point p = GetRenderingPosition();
		render->Draw(p.x, p.y);
	}
	else {
		isActive = false;
	}
}
void EnemyZombie::Reset()
{
}
void EnemyZombie::Brain()
{
	MoveX();
	if (this->GetHitbox().TestAABB(EnemyManager::Instance().target->GetHitbox())) {
		DamagePlayer();
	}
	if (this->GetHitbox().TestAABB(EnemyManager::Instance().target->weapon->HitboxOnAttack())) {
		AudioPlayer::Instance().PlaySoundByName("Attack");
		isActive = false;
		EnemyManager::Instance().target->IncrScore(100);
	}
}
void EnemyZombie::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
void EnemyZombie::AdvanceRight()
{
	state = EnemyState::ADVANCING;
	look = EnemyLook::RIGHT;
	SetAnimation((int)EnemyAnim::ADVANCING_RIGHT);
}
void EnemyZombie::AdvanceLeft()
{
	state = EnemyState::ADVANCING;
	look = EnemyLook::LEFT;
	SetAnimation((int)EnemyAnim::ADVANCING_LEFT);
}
void EnemyZombie::MoveX()
{
	if (state == EnemyState::IDLE) {
		if (look == EnemyLook::RIGHT) {
			AdvanceRight();
		}
		else {
			AdvanceLeft();
		}
	}
	else if(state == EnemyState::ADVANCING){
		if (look == EnemyLook::RIGHT) {
			pos.x += ZOMBIE_SPEED;
		}
		else {

			pos.x -= ZOMBIE_SPEED;
		}
	}
	else if (state == EnemyState::DEAD) {
		SetAnimation((int)EnemyAnim::EMPTY);
	}

}
void EnemyZombie::DrawDebug(const Color& col) const
{
	DrawHitbox(BLUE);
}
void EnemyZombie::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_ZOMBIE);

	render->Release();
}

