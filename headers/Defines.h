#pragma once

#include <KamskiEngine/engine/deps/glm/glm.hpp>

#include "KamskiEngine/KamskiDefines.h"
#include "KamskiEngine/KamskiLog.h"
#include "KamskiEngine/KamskiMemory.h"
#include "KamskiEngine/KamskiRenderer.h"

class GameState;

inline GameState* GAME_STATE;
inline const GameLog* LOGGER;
inline const GameRenderer* RENDERER;
inline GameMemory* MEMORY;

using texture_id = u32;

enum class TextureTag: u8
{
    ENEMY,
    PLAYER,
    PROJECTILE,
    /* MAP TILES, RATIO ALWAYS 1:1 SO ADD THEM ONLY AFTER THIS COMMENT */
    FLOOR,
    WALL,
    WALL_BOTTOM,
    WALL_TOP,
    WALL_LEFT,
    WALL_RIGHT,
    WALL_CORNER_BOTTOM_LEFT,
    WALL_CORNER_BOTTOM_RIGHT,
    WALL_CORNER_TOP_LEFT,
    WALL_CORNER_TOP_RIGHT,
    COUNT
};

#define TAG(TEXTURE) ((u32)TextureTag:: TEXTURE)
constexpr glm::vec2 TEXTURE_SIZES[] = {
    [TAG(ENEMY)]      = {0.25f, 0.25f},
    [TAG(PLAYER)]     = {0.125f, 0.25f},
    [TAG(PROJECTILE)] = {0.06f, 0.13f}
};

inline constexpr u32 MAP_SIZE = 20;
inline constexpr u32 MAP_SIZE_Y = MAP_SIZE;
inline constexpr u32 MAP_SIZE_X = MAP_SIZE;
inline constexpr f32 PI = 3.14159265f;

//inline constexpr u32 MAX_NUMBER_OF_PLAYERS = 2;

// concatenating assets' dir with filename
#define ASSET(filename) ("assets/" filename)
inline const char* TEXTURE_PATHS[] = {
    [TAG(FLOOR)]                    = ASSET("floor.png"),
    [TAG(WALL)]                     = ASSET("wall_mid.png"),
    [TAG(WALL_BOTTOM)]              = ASSET("wall_bottom_mid.png"),
    [TAG(WALL_TOP)]                 = ASSET("wall_top_mid.png"),
    [TAG(WALL_LEFT)]                = ASSET("wall_left.png"),
    [TAG(WALL_RIGHT)]               = ASSET("wall_right.png"),
    [TAG(WALL_CORNER_BOTTOM_LEFT)]  = ASSET("wall_corner_bottom_left.png"),
    [TAG(WALL_CORNER_BOTTOM_RIGHT)] = ASSET("wall_corner_bottom_right.png"),
    [TAG(WALL_CORNER_TOP_LEFT)]     = ASSET("wall_corner_top_left.png"),
    [TAG(WALL_CORNER_TOP_RIGHT)]    = ASSET("wall_corner_top_right.png"),
    [TAG(ENEMY)]                    = ASSET("enemy.png"),
    [TAG(PLAYER)]                   = ASSET("player.png"),
    [TAG(PROJECTILE)]               = ASSET("weapon_knife.png")
};
inline constexpr u32 TEXTURE_COUNT = ARRAY_COUNT(TEXTURE_PATHS);

#undef TAG
#undef ASSET
#define KASMKI_MAX_ENTITY_COUNT 20000
#define KAMSKI_COMPONENTS SpriteComponent, TagComponent, SolidColorComponent, FollowComponent, EntityComponent, ProjectileComponent, HealthBarComponent
