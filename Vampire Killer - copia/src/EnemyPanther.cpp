
#include "EnemyPanther.h"
#include "EnemyManager.h"

EnemyPanther::EnemyPanther(Point pos) : Enemy(pos, PANTHER_HITBOX_HEIGHT, PANTHER_HITBOX_WIDTH, PANTHER_SPRITE_HEIGHT, PANTHER_SPRITE_WIDTH)
{
	state = EnemyState::ADVANCING;
	look = EnemyLook::RIGHT;
	int AnimationFrame = 0;
	Damage = PANTHER_DAMAGE;

	Initialise();
}
EnemyPanther::~EnemyPanther()
{
}
AppStatus EnemyPanther::Initialise()
{

	int i;
	const float n = (float)PANTHER_SPRITE_HEIGHT;
	const float n2 = (float)PANTHER_SPRITE_WIDTH;
	AnimationFrame = 0;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_PANTHER, "images/Spritesheets/Enemies & Characters/Enemy 4 Sprite Sheet.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(Resource::IMG_PANTHER));
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
void EnemyPanther::Update()
{
	Brain();
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void EnemyPanther::Render()
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
void EnemyPanther::Reset()
{
}
void EnemyPanther::Brain()
{
	MoveX();
	if (this->GetHitbox().TestAABB(EnemyManager::Instance().target->GetHitbox())) {
		DamagePlayer();
	}
}
void EnemyPanther::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
void EnemyPanther::AdvanceRight()
{
	state = EnemyState::ADVANCING;
	look = EnemyLook::RIGHT;
	SetAnimation((int)EnemyAnim::ADVANCING_RIGHT);
}
void EnemyPanther::AdvanceLeft()
{
	state = EnemyState::ADVANCING;
	look = EnemyLook::LEFT;
	SetAnimation((int)EnemyAnim::ADVANCING_LEFT);
}
void EnemyPanther::MoveX()
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
		if (look == EnemyLook::RIGHT) {
			pos.x += PANTHER_SPEED;
		}
		else {

			pos.x -= PANTHER_SPEED;
		}
	}
	else if (state == EnemyState::DEAD) {
		SetAnimation((int)EnemyAnim::EMPTY);
	}
}
void EnemyPanther::DrawDebug(const Color& col) const
{
	DrawHitbox(BLUE);
}
void EnemyPanther::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PANTHER);

	render->Release();
}

