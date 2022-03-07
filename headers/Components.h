#pragma once

#include "Defines.h"

struct SpriteComponent
{
    f32 x;
    f32 y;
    texture_id textureId;
};

struct EntityComponent
{
    f32 movementSpeed;
    u32 healthPoints;
    u32 attackPoints;
};
