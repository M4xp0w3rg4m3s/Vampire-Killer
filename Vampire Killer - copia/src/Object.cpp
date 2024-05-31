#include "Object.h"
#include "StaticImage.h"
#include "Sprite.h"

Object::Object(const Point& p, ObjectType t, Vector2 levelIndex) : Entity(p, OBJECT_PHYSICAL_SIZE, OBJECT_PHYSICAL_SIZE, OBJECT_FRAME_SIZE, OBJECT_FRAME_SIZE)
{
    type = t;

    Rectangle rc;
    const int n = TILE_SIZE;

    ResourceManager& data = ResourceManager::Instance();

    render2 = new Sprite(data.GetTexture(Resource::IMG_TILES));
    if (render2 == nullptr)
    {
        LOG("Failed to allocate memory for objects sprites");
    }

    Sprite* sprite = dynamic_cast<Sprite*>(render2);
    sprite->SetNumberAnimations(2);

    sprite->SetAnimationDelay((int)ObjectAnim::FIRE, ANIM_DELAY);
    sprite->AddKeyFrame((int)ObjectAnim::FIRE, { 11 * n, 4 * n, n, n });
    sprite->AddKeyFrame((int)ObjectAnim::FIRE, { 12 * n, 4 * n, n, n });
    sprite->SetAnimationDelay((int)ObjectAnim::CANDLE, ANIM_DELAY);
    sprite->AddKeyFrame((int)ObjectAnim::CANDLE, { 9 * n, 4 * n, n, n });
    sprite->AddKeyFrame((int)ObjectAnim::CANDLE, { 10 * n, 4 * n, n, n });

    switch (type)
    {
    case ObjectType::CHAIN:         rc = { 15 * n,  4 * n, n, n };      
        render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
        break;
    case ObjectType::SHIELD:        rc = { 14 * n,  7 * n, n, n };      
        render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
        break;
    case ObjectType::KEY_CHEST:     rc = { 12 * n,  7 * n, n, n };      
        render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
        break;
    case ObjectType::KEY_DOOR:      rc = { 13 * n,  7 * n, n, n };      
        render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
        break;
    case ObjectType::CHEST_CHAIN:   rc = { 11 * n,  7 * n, n, n };      
        render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
        break;
    case ObjectType::CHEST_SHIELD:  rc = { 11 * n,  7 * n, n, n };      
        render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
        break;
    case ObjectType::CHEST_BOOTS:   rc = { 11 * n,  7 * n, n, n };      
        render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
        break;
    case ObjectType::CHEST_WINGS:   rc = { 11 * n,  7 * n, n, n };      
        render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
        break;
    case ObjectType::CHEST_HEART:   rc = { 11 * n,  7 * n, n, n };      
        render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
        break;
    case ObjectType::HEART_BIG:     rc = { 13 * n,  4 * n, n, n };      
        render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
        break;
    case ObjectType::HEART_SMALL:   rc = { 14 * n,  4 * n, n, n };      
        render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
        break;
    case ObjectType::BOOTS:         rc = { 15 * n,  7 * n, n, n };     
        render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
        break;
    case ObjectType::WINGS:         rc = { 0 * n,  8 * n, n, n };       
    render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
    break;

    case ObjectType::FIRE:          sprite->SetAnimation((int)ObjectAnim::FIRE);          break;
    case ObjectType::CANDLE:        sprite->SetAnimation((int)ObjectAnim::CANDLE);        break;

    default: LOG("Internal error: object creation of invalid type (Normal Item)");
    }

    levelContainer = levelIndex;
}
Object::Object(const Point& p, ObjectType t, Vector2 levelIndex, ObjectType loot) : Entity(p, OBJECT_PHYSICAL_SIZE, OBJECT_PHYSICAL_SIZE, OBJECT_FRAME_SIZE, OBJECT_FRAME_SIZE)
{
    type = t;

    Rectangle rc;
    const int n = TILE_SIZE;

    ResourceManager& data = ResourceManager::Instance();

    render2 = new Sprite(data.GetTexture(Resource::IMG_TILES));
    if (render2 == nullptr)
    {
        LOG("Failed to allocate memory for player sprite");
    }

    Sprite* sprite = dynamic_cast<Sprite*>(render2);
    sprite->SetNumberAnimations(2);

    sprite->SetAnimationDelay((int)ObjectAnim::FIRE, ANIM_DELAY);
    sprite->AddKeyFrame((int)ObjectAnim::FIRE, { 11 * n, 4 * n, n, n });
    sprite->AddKeyFrame((int)ObjectAnim::FIRE, { 12 * n, 4 * n, n, n });
    sprite->SetAnimationDelay((int)ObjectAnim::CANDLE, ANIM_DELAY);
    sprite->AddKeyFrame((int)ObjectAnim::CANDLE, { 9 * n, 4 * n, n, n });
    sprite->AddKeyFrame((int)ObjectAnim::CANDLE, { 10 * n, 4 * n, n, n });

    switch (type)
    {
    case ObjectType::CHAIN:         rc = { 15 * n,  4 * n, n, n };      
        render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
        break;
    case ObjectType::SHIELD:        rc = { 14 * n,  7 * n, n, n };      
        render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
        break;
    case ObjectType::KEY_CHEST:     rc = { 12 * n,  7 * n, n, n };      
        render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
        break;
    case ObjectType::KEY_DOOR:      rc = { 13 * n,  7 * n, n, n };      
        render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
        break;
    case ObjectType::CHEST_CHAIN:   rc = { 11 * n,  7 * n, n, n };      
        render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
        break;
    case ObjectType::CHEST_SHIELD:  rc = { 11 * n,  7 * n, n, n };      
        render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
        break;
    case ObjectType::CHEST_BOOTS:   rc = { 11 * n,  7 * n, n, n };      
        render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
        break;
    case ObjectType::CHEST_WINGS:   rc = { 11 * n,  7 * n, n, n };      
        render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
        break;
    case ObjectType::CHEST_HEART:   rc = { 11 * n,  7 * n, n, n };      
        render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
        break;
    case ObjectType::HEART_BIG:     rc = { 13 * n,  4 * n, n, n };      
        render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
        break;
    case ObjectType::HEART_SMALL:   rc = { 14 * n,  4 * n, n, n };      
        render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
        break;
    case ObjectType::BOOTS:         rc = { 15 * n,  7 * n, n, n };     
        render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
        break;
    case ObjectType::WINGS:         rc = { 0 * n,  8 * n, n, n };       
    render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
    break;

    case ObjectType::FIRE:          sprite->SetAnimation((int)ObjectAnim::FIRE);          break;
    case ObjectType::CANDLE:        sprite->SetAnimation((int)ObjectAnim::CANDLE);        break;

    default: LOG("Internal error: object creation of invalid type (Normal Item)");
    }

    levelContainer = levelIndex;
    containedLoot = loot;
}
Object::~Object()
{
    if (type != ObjectType::FIRE && type != ObjectType::CANDLE) {
        render->Release();
    }
    render2->Release();
    delete[] render2;
}
void Object::Update()
{
    Sprite* sprite = dynamic_cast<Sprite*>(render2);
    sprite->Update();
}
void Object::DrawDebug(const Color& col) const
{
    Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
void Object::DrawAnimation() const
{
    Sprite* sprite = dynamic_cast<Sprite*>(render2);
    sprite->Draw(pos.x,pos.y-TILE_SIZE+1);
}
int Object::Points() const
{
    return 0;
}

ObjectType Object::GetType() const
{
    return type;
}

ObjectType Object::GetLoot() const
{
    return containedLoot;
}

Vector2 Object::GetObjectLevel() const
{
    return levelContainer;
}
