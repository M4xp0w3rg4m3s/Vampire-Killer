
#include "EnemyBat.h"
#include "EnemyManager.h"



EnemyBat::EnemyBat(Point pos) : Enemy(pos, BAT_HITBOX_HEIGHT, BAT_HITBOX_WIDTH, BAT_SPRITE_HEIGHT, BAT_SPRITE_WIDTH)
{
	state = EnemyState::ADVANCING;
	look = EnemyLook::RIGHT;
	int AnimationFrame = 0;
	Damage = BAT_DAMAGE;

	Initialise();
}
EnemyBat::~EnemyBat()
{
}
AppStatus EnemyBat::Initialise()
{
	if (EnemyManager::Instance().target->GetPos().x > WINDOW_WIDTH) {
		SetPos({ 255, 176 });
		Init_pos_y = pos.y;
	}
	else if (EnemyManager::Instance().target->GetPos().x > WINDOW_WIDTH) {
		SetPos({ 20, 176 });
	}

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

	state = EnemyState::IDLE;
	look = EnemyLook::RIGHT;
	SetAnimation((int)EnemyAnim::ADVANCING_RIGHT);

	return AppStatus::OK;
}
void EnemyBat::Update()
{
	Brain();
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void EnemyBat::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
void EnemyBat::Render()
{
	if (pos.x > 16 && pos.x < 256)
	{
		Point p = GetRenderingPosition();
		render->Draw(p.x, p.y);
	}
	else {
		EnemyManager::Instance().DestroyEnemies();
	}
}
void EnemyBat::Reset()
{
}
void EnemyBat::Brain()
{
	Move();
	if (this->GetHitbox().TestAABB(EnemyManager::Instance().target->GetHitbox())) {
		DamagePlayer();
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
	Wave w{ Init_pos_y,40,80,20};
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
			w.alpha -= w.delta;
			pos.y += w.y0 + w.amplitude * sin(pos.y * w.alpha );
		/*	w.alpha -= w.delta;
			pos.y += w.y0 + w.amplitude * sin(w.alpha * pos.y);*/
			
			pos.x += BAT_SPEED;
		}
		else {
			w.alpha -= w.delta;
			pos.y += w.y0 + w.amplitude * sin(w.alpha );
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

