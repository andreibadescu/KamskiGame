#pragma once
#include "Defines.h"
#include "KamskiEngine/KamskiMeta.h"
using EntityComponent = EntityStats;

component TransformComponent
{
    glm::vec2 position;
    glm::vec2 size;
    f32 rotation;
};

component TypeComponent
{
    EntityType entityType;
};

component ColliderComponent
{
    glm::vec2 hitBox;
};

component SpriteComponent
{
    AnimationTag animation;
    f64 startTime;
    f64 endTime;
};

component SolidColorComponent
{
    glm::vec4 color;
};

enum Phase
{
    WALK,
    SHOOT,
    WAIT,
    STUNNED
};

component EnemyComponent
{
    Phase phase;
    f32 startTime;
};

enum Effect
{
    EFFECT_ON_FIRE,
    EFFECT_COUNT
};

component EffectComponent
{
    f32 effectDuration[EFFECT_COUNT];
    bool effects[EFFECT_COUNT];
};

component VelocityComponent
{
    glm::vec2 targetVel;
    glm::vec2 vel;
};

component FollowComponent
{
    Entity toFollowId;
    glm::vec2 followOffset;
};

component ProjectileComponent
{
    glm::vec2 direction;
    f32 speed;
    f32 damage;
    bool isEnemy;
};

component HealthBarComponent
{
    f32 maxSize;
    f32 maxHealth;
};

component ItemComponent
{
    // Weapon, Armor, Utility
    ItemType itemType;
    // Bow sword, shield, helmet, etc.
    ItemBit itemId;
};
