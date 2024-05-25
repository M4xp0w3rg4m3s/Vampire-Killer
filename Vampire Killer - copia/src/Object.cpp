#include "Object.h"
#include "StaticImage.h"

Object::Object(const Point& p, ObjectType t) : Entity(p, OBJECT_PHYSICAL_SIZE, OBJECT_PHYSICAL_SIZE, OBJECT_FRAME_SIZE, OBJECT_FRAME_SIZE)
{
    type = t;

    Rectangle rc;
    const int n = TILE_SIZE;
    switch (type)
    {
    case ObjectType::CHAIN: rc = { 15 * n,  4 * n, n, n }; break;
    case ObjectType::CHEST_CHAIN: rc = { 11 * n,  7 * n, n, n }; break;

    default: LOG("Internal error: object creation of invalid type");
    }

    ResourceManager& data = ResourceManager::Instance();
    render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);

    data.LoadTexture(Resource::IMG_OPEN_CHEST, "images/Spritesheets/FX/OpenChest.png");
    chest_animation = data.GetTexture(Resource::IMG_OPEN_CHEST);

    chest_time = 180;
}
Object::~Object()
{
    ResourceManager& data = ResourceManager::Instance();
    data.ReleaseTexture(Resource::IMG_OPEN_CHEST);

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

void Object::OpenChest(ObjectType chestType)
{
    chest_time--;

    // PLAY ANIMATION
    if (chest_time == 0) {
        if (chestType == ObjectType::CHEST_CHAIN) {
            type = ObjectType::CHAIN;
        }
    }
    else {
        if (chest_time % 30 < 8) {
            DrawTextureRec(*chest_animation, { 0,0,16,16 }, { (float)this->pos.x,(float)this->pos.y }, WHITE);
        }
        else if (chest_time % 30 < 16) {
            DrawTextureRec(*chest_animation, { 16 * 1,0,16,16 }, { (float)this->pos.x,(float)this->pos.y }, WHITE);
        }
        else if (chest_time % 30 < 23) {
            DrawTextureRec(*chest_animation, { 16 * 2,0,16,16 }, { (float)this->pos.x,(float)this->pos.y }, WHITE);
        }
        else if (chest_time % 30 < 30) {
            DrawTextureRec(*chest_animation, { 16 * 3,0,16,16 }, { (float)this->pos.x,(float)this->pos.y }, WHITE);
        }
    }
}

int Object::GetChestTime() const
{
    return chest_time;
}
