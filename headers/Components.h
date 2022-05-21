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
    Entity shooterId;
};

struct HealthBarComponent
{
    glm::vec2 maxSize;
    f32 maxHealth;
};
