#include "Object.h"
#include "StaticImage.h"

Object::Object(const Point& p, ObjectType t, Vector2 levelIndex) : Entity(p, OBJECT_PHYSICAL_SIZE, OBJECT_PHYSICAL_SIZE, OBJECT_FRAME_SIZE, OBJECT_FRAME_SIZE)
{
    type = t;

    Rectangle rc;
    const int n = TILE_SIZE;
    switch (type)
    {
    case ObjectType::CHAIN: rc = { 15 * n,  4 * n, n, n }; break;
    case ObjectType::SHIELD: rc = { 14 * n,  7 * n, n, n }; break;

    case ObjectType::KEY_CHEST: rc = { 12 * n,  7 * n, n, n }; break;
    case ObjectType::KEY_DOOR: rc = { 13 * n,  7 * n, n, n }; break;

    case ObjectType::CHEST_CHAIN: rc = { 11 * n,  7 * n, n, n }; break;
    case ObjectType::CHEST_SHIELD: rc = { 11 * n,  7 * n, n, n }; break;

    default: LOG("Internal error: object creation of invalid type");
    }

    ResourceManager& data = ResourceManager::Instance();
    render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);


    levelContainer = levelIndex;
}
Object::~Object()
{
    render->Release();
}
void Object::DrawDebug(const Color& col) const
{
    Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
int Object::Points() const
{
    return 0;
}

ObjectType Object::GetType() const
{
    return type;
}

Vector2 Object::GetObjectLevel() const
{
    return levelContainer;
}
