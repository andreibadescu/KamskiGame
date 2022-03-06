#pragma once

#include "KamskiEngine/KamskiDefines.h"

using texture_id = u32;

enum class TextureTag : u8
{
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

inline constexpr u32 MAP_SIZE = 20;
inline constexpr u32 MAP_SIZE_Y = MAP_SIZE;
inline constexpr u32 MAP_SIZE_X = MAP_SIZE;

// concatenating assets dir with filename
#define ASSET(filename) ("assets/"##filename)
inline const char* TEXTURE_PATHS[] = {
    ASSET("floor.png"),
    ASSET("wall_mid.png"),
    ASSET("wall_bottom_mid.png"),
    ASSET("wall_top_mid.png"),
    ASSET("wall_left.png"),
    ASSET("wall_right.png"),
    ASSET("wall_corner_bottom_left.png"),
    ASSET("wall_corner_bottom_right.png"),
    ASSET("wall_corner_top_left.png"),
    ASSET("wall_corner_top_right.png")
};
constexpr u32 TEXTURE_COUNT = ARRAY_COUNT(TEXTURE_PATHS);
