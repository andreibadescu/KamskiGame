#pragma once

#include <KamskiEngine/engine/deps/glm/glm.hpp>

#include "KamskiEngine/KamskiDefines.h"
#include "KamskiEngine/KamskiEngine.h"

class GameState;

inline GameState* GAME_STATE;
inline const GameLog* LOGGER;
inline const GameRenderer* RENDERER;
inline const GameUI* UI;
inline GameMemory* MEMORY;
inline GameIO* IO;
inline GameAnimation* ANIMATION;

using texture_id = u32;

enum class TextureTag: u8
{
    ENEMY,
    PLAYER,
    PROJECTILE,
    CIRCLE,
    BUTTON,
    BUTTON_PRESSED,
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
    [TAG(ENEMY)]      = {250.0f, 250.0f},
    [TAG(PLAYER)]     = {125.0f, 125.0f},
    [TAG(PROJECTILE)] = {60.0f, 130.0f},
    [TAG(CIRCLE)] = {100.0f, 100.0f}
};

inline constexpr u32 MAP_SIZE = 20;
inline constexpr u32 MAP_SIZE_Y = MAP_SIZE;
inline constexpr u32 MAP_SIZE_X = MAP_SIZE;
//TODO: create a math library
inline constexpr f32 PI = 3.14159265f;

//inline constexpr u32 MAX_NUMBER_OF_PLAYERS = 2;

// concatenating assets' dir with filename
#define TEXTURE_ASSET(filename) ("assets/" filename)
inline const char* TEXTURE_PATHS[] = {
    [TAG(FLOOR)]                    = TEXTURE_ASSET("floor.png"),
    [TAG(WALL)]                     = TEXTURE_ASSET("wall_mid.png"),
    [TAG(WALL_BOTTOM)]              = TEXTURE_ASSET("wall_bottom_mid.png"),
    [TAG(WALL_TOP)]                 = TEXTURE_ASSET("wall_top_mid.png"),
    [TAG(WALL_LEFT)]                = TEXTURE_ASSET("wall_left.png"),
    [TAG(WALL_RIGHT)]               = TEXTURE_ASSET("wall_right.png"),
    [TAG(WALL_CORNER_BOTTOM_LEFT)]  = TEXTURE_ASSET("wall_corner_bottom_left.png"),
    [TAG(WALL_CORNER_BOTTOM_RIGHT)] = TEXTURE_ASSET("wall_corner_bottom_right.png"),
    [TAG(WALL_CORNER_TOP_LEFT)]     = TEXTURE_ASSET("wall_corner_top_left.png"),
    [TAG(WALL_CORNER_TOP_RIGHT)]    = TEXTURE_ASSET("wall_corner_top_right.png"),
    [TAG(ENEMY)]                    = TEXTURE_ASSET("enemy.png"),
    [TAG(PLAYER)]                   = TEXTURE_ASSET("player.png"),
    [TAG(PROJECTILE)]               = TEXTURE_ASSET("weapon_knife.png"),
    [TAG(CIRCLE)]                   = TEXTURE_ASSET("circle.png"),
    [TAG(BUTTON)]                   = TEXTURE_ASSET("menu_button.png"),
    [TAG(BUTTON_PRESSED)]           = TEXTURE_ASSET("menu_button_press.png")
};
inline constexpr u32 TEXTURE_COUNT = ARRAY_COUNT(TEXTURE_PATHS);

#undef TAG
#undef TEXTURE_ASSET
#define KASMKI_MAX_ENTITY_COUNT 20000
#define KAMSKI_COMPONENTS SpriteComponent, TagComponent, SolidColorComponent, FollowComponent, EntityComponent, ProjectileComponent, HealthBarComponent
