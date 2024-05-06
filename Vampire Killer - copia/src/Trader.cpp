#include "Trader.h"

Trader::Trader(Point pos) : Entity(pos, TRADER_HITBOX_HEIGHT, TRADER_HITBOX_WIDTH, TRADER_SPRITE_HEIGHT, TRADER_SPRITE_WIDTH)
{
	int AnimationFrame = 0;

	Initialise();
}
Trader::~Trader()
{
	Release();
}
AppStatus Trader::Initialise()
{
	if (Trader::GetPos().x > WINDOW_WIDTH) {
		SetPos({ 255, 176 });
	}
	else if (Trader::GetPos().x > WINDOW_WIDTH) {
		SetPos({ 20, 176 });
	}

	int i;
	const float n = (float)TRADER_SPRITE_HEIGHT;
	const float n2 = (float)TRADER_SPRITE_WIDTH;
	AnimationFrame = 0;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_ZOMBIE, "images/Spritesheets/Enemies & Characters/Trader Sprite Sheet.png") != AppStatus::OK)
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

	state = EnemyState::IDLE;
	SetAnimation((int)EnemyAnim::ADVANCING_LEFT);

	return AppStatus::OK;
}
void Trader::Update()
{
	Brain();
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void Trader::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
void Trader::Render()
{
	if (pos.x > 16 && pos.x < 256)
	{
		Point p = GetRenderingPosition();
		render->Draw(p.x, p.y);
	}
	else {
		Trader::Release();
	}
}
void Trader::Reset()
{
}
void Trader::Brain()
{
	
}
void Trader::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
void Trader::DrawDebug(const Color& col) const
{
	DrawHitbox(ORANGE);
}
void Trader::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_ZOMBIE);

	render->Release();
}


