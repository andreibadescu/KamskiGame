#pragma once

#include "Defines.h"

/* TO BE REFACTORED */

//struct TransformComponent
//{
    //glm::vec2 position;
    //glm::vec2 size;
    //glm::vec2 rotation;
//};

enum class TagComponent
{
    PLAYER,
    ENEMY
};

struct SpriteComponent
{
    glm::vec2 position;
    glm::vec2 size;
    texture_id textureId;
    f32 rotation;
    u8 animationState;
};

struct SolidColorComponent
{
    glm::vec2 position;
    glm::vec2 size;
    glm::vec4 color;
    f32 rotation;
};

struct EnemyComponent
{
    enum Phase
    {
        WALK,
        SHOOT,
        WAIT
    }phase;
    f32 startTime;
};

struct VelocityComponent
{
    glm::vec2 targetVel;
    glm::vec2 vel;
};

struct FollowComponent
{
    Entity toFollowId;
    glm::vec2 followOffset;
};

struct EntityComponent
{
    f32 movementSpeed;
    f32 healthPoints;
    f32 attackPoints;
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
    glm::vec2 maxSize;
    f32 maxHealth;
};

struct ItemComponent
{
    // Weapon, Armor, Utility
    ItemType itemType;
    // Bow sword, shield, helmet, etc.
    ItemBit itemId;
};
