#include "Trader.h"


Trader::Trader(Point pos) : Enemy(pos, TRADER_HITBOX_HEIGHT, TRADER_HITBOX_WIDTH, TRADER_SPRITE_HEIGHT, TRADER_SPRITE_WIDTH)
{
	int AnimationFrame = 0;
	type = EnemyType::TRADER;

	Initialise();
}
Trader::~Trader()
{
	Release();
}
AppStatus Trader::Initialise()
{
	PopUp = false;


	int i;
	const float n = (float)TRADER_SPRITE_HEIGHT;
	const float n2 = (float)TRADER_SPRITE_WIDTH;
	AnimationFrame = 0;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_TRADER, "images/Spritesheets/Enemies & Characters/Trader Sprite Sheet.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}


	render = new Sprite(data.GetTexture(Resource::IMG_TRADER));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)EnemyAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)EnemyAnim::IDLE_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::IDLE_LEFT, { 0, 0, n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::RED_TRADER, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::RED_TRADER, { n, 0, n , n });

	SetAnimation((int)EnemyAnim::IDLE_LEFT);

	return AppStatus::OK;
}
void Trader::Update()
{
	if (justHit) {
		counter--;
	}
	if (counter <= 0 || !justHit) {
		Brain();
		justHit = false;
	}
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void Trader::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
bool Trader::GetPopUp() const
{
	return PopUp;
}
void Trader::Render()
{
	if (isActive) {
		Point p = GetRenderingPosition();
		render->Draw(p.x, p.y);
	}
}
void Trader::Reset()
{
}
void Trader::Brain()
{
	if (currentAnimation == EnemyAnim::RED_TRADER && this->GetHitbox().TestAABB(EnemyManager::Instance().target->weapon->HitboxOnAttack()))
	{
		isActive = false;
		justHit = true;
		//counter = 60;
	}
	else if (this->GetHitbox().TestAABB(EnemyManager::Instance().target->weapon->HitboxOnAttack()) && PopUp == false)
	{
		AudioPlayer::Instance().PlaySoundByName("Attack");
		PopUp = true;
		justHit = true;

	}
	else if (this->GetHitbox().TestAABB(EnemyManager::Instance().target->weapon->HitboxOnAttack()) && PopUp == true && EnemyManager::Instance().target->GetHearts() > 90)
	{
		PopUp = false;
		SetAnimation((int)EnemyAnim::RED_TRADER);
		currentAnimation = EnemyAnim::RED_TRADER;
		EnemyManager::Instance().target->DecrHearts(90);
		EnemyManager::Instance().target->weapon->SetWeapon(WeaponType::CHAIN); // Put Knives when finished
		justHit = true;
		//counter = 60;
	}
	counter = 60;
}
void Trader::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
void Trader::DrawDebug(const Color& col) const
{
	DrawHitbox(WHITE);
}
void Trader::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_TRADER);

	render->Release();
}