//
//#include "EnemyZombie.h"
//
//EnemyZombie::EnemyZombie()
//{
//	EnemyState state = EnemyState::ADVANCING;
//	int AnimationFrame = 0;
//	int EnemySpeed = ZOMBIE_SPEED;
//
//	int EnemyFrameHeight = ZOMBIE_SPRITE_HEIGHT;
//	int EnemyFrameWidth = ZOMBIE_SPRITE_WIDTH;
//
//	int EnemyHitboxHeight = ZOMBIE_HITBOX_HEIGHT;
//	int EnemyHitboxWidth = ZOMBIE_HITBOX_WIDTH;
//}
//EnemyZombie::~EnemyZombie()
//{
//
//}
//AppStatus EnemyZombie::Initialise()
//{
//	int i;
//	const float n = (float)EnemyFrameHeight;
//	const float n2 = (float)EnemyFrameWidth;
//	AnimationFrame = 0;
//
//	ResourceManager& data = ResourceManager::Instance();
//	if (data.LoadTexture(Resource::IMG_ZOMBIE, "images/Spritesheets/Enemies & Characters/Enemy 1 Sprite Sheet.png") != AppStatus::OK)
//	{
//		return AppStatus::ERROR;
//	}
//
//	render = new Sprite(data.GetTexture(Resource::IMG_ZOMBIE));
//	if (render == nullptr)
//	{
//		LOG("Failed to allocate memory for player sprite");
//		return AppStatus::ERROR;
//	}
//
//	Sprite* sprite = dynamic_cast<Sprite*>(render);
//	sprite->SetNumberAnimations((int)EnemyAnim::NUM_ANIMATIONS);
//
//	sprite->SetAnimationDelay((int)EnemyAnim::ADVANCING_RIGHT, ANIM_DELAY);
//	for (i = 0; i < 2; ++i)
//		sprite->AddKeyFrame((int)EnemyAnim::ADVANCING_RIGHT, { (float)i * n, 0, n, n });
//	sprite->SetAnimationDelay((int)EnemyAnim::ADVANCING_LEFT, ANIM_DELAY);
//	for (i = 0; i < 8; ++i)
//		sprite->AddKeyFrame((int)EnemyAnim::ADVANCING_LEFT, { (float)i * n, 0, -n, n });
//
//	return AppStatus::OK;
//}
//void EnemyZombie::Update()
//{
//	MoveX();
//	Sprite* sprite = dynamic_cast<Sprite*>(render);
//	sprite->Update();
//}
//void EnemyZombie::SetAnimation(int id)
//{
//	Sprite* sprite = dynamic_cast<Sprite*>(render);
//	sprite->SetAnimation(id);
//}
//void EnemyZombie::Render()
//{
//
//}
//void EnemyZombie::AdvanceRight()
//{
//	state = EnemyState::ADVANCING;
//	look = EnemyLook::RIGHT;
//	SetAnimation((int)EnemyAnim::ADVANCING_RIGHT);
//}
//void EnemyZombie::AdvanceLeft()
//{
//	state = EnemyState::ADVANCING;
//	look = EnemyLook::LEFT;
//	SetAnimation((int)EnemyAnim::ADVANCING_LEFT);
//}
//void EnemyZombie::MoveX()
//{
//	if (look == EnemyLook::RIGHT) {
//		AdvanceRight();
//		pos.x = (pos.x + 1) * EnemySpeed;
//	}
//	else {
//		AdvanceLeft();
//		pos.x = (pos.x - 1) * EnemySpeed;
//	}
//	if (map->TestCollisionLeft(GetHitbox()) || map->TestCollisionRight(GetHitbox())
//	|| map->TestCollisionWallRight(GetHitbox()) || map->TestCollisionWallRight(GetHitbox())
//	|| map->TestCollisionWin(GetHitbox()))
//	{
//		Release();
//	}
//}
//void EnemyZombie::Spawn()
//{
//
//}
//void EnemyZombie::DrawDebug(const Color& col) const
//{
//	DrawHitbox(BLUE);
//}
//void EnemyZombie::Release()
//{
//	ResourceManager& data = ResourceManager::Instance();
//	data.ReleaseTexture(Resource::IMG_PLAYER);
//
//	render->Release();
//}
//
