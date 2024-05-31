#pragma once
#include "Entity.h"

#define OBJECT_PHYSICAL_SIZE    14
#define OBJECT_FRAME_SIZE        16

#define POINTS_HEART    0

enum class ObjectType { 
    CHAIN, SHIELD, BOOTS, WINGS, 
    KEY_CHEST, KEY_DOOR,
    CHEST_CHAIN, CHEST_SHIELD, CHEST_BOOTS, CHEST_WINGS, CHEST_HEART,
    HEART_SMALL, HEART_BIG,
    FIRE, CANDLE };

enum class ObjectAnim { FIRE, CANDLE };

class Object : public Entity
{
public:
    Object(const Point& p, ObjectType t, Vector2 levelIndex);
    Object(const Point& p, ObjectType t, Vector2 levelIndex, ObjectType loot);
    ~Object();

    void Update();
    void DrawDebug(const Color& col) const;
    void DrawAnimation() const;
    int Points() const;

    ObjectType GetType()const;
    ObjectType GetLoot()const;

    Vector2 GetObjectLevel() const;

private:
    ObjectType type;
    ObjectType containedLoot;
    Vector2 levelContainer;

    RenderComponent* render2;

};