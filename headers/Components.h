#pragma once
#include "Defines.h"

struct TransformComponent
{
    glm::vec2 position;
    glm::vec2 size;
    f32 rotation;
};

struct TypeComponent
{
    EntityType entityType;
};

struct ColliderComponent
{
    glm::vec2 hitBox;
};

struct SolidColorComponent
{
    glm::vec4 color;
};

struct VelocityComponent
{
    glm::vec2 targetVel;
    glm::vec2 vel;
};

struct StatsComponent
{
    f32 movementSpeed;
    f32 healthPoints;
    f32 attackPoints;
};

struct FollowComponent
{
    Entity toFollowId;
    glm::vec2 followOffset;
};

struct ProjectileComponent
{
    glm::vec2 direction;
    f32 speed;
    f32 damage;
    bool isEnemy;
};

struct HealthBarComponent
{
    f32 maxSize;
    f32 maxHealth;
};

struct ItemComponent
{
    // Weapon, Armor, Utility
    ItemType itemType;
    // Bow sword, shield, helmet, etc.
    ItemBit itemId;
};

#define KAMSKI_COMPONENTS \
TransformComponent,\
TypeComponent, \
ColliderComponent, \
SolidColorComponent, \
FollowComponent, \
StatsComponent, \
ProjectileComponent, \
HealthBarComponent, \
ItemComponent, \
VelocityComponent
