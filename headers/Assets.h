#pragma once
#include "../../KamskiEngine/KamskiTypes.h"

// NOTE(Phillip): Everytime you add a new enum, assign it a unique value
enum AssetTag
{
    ASSET_TAG_BUTTON = 0,
    ASSET_TAG_CHEST = 1,
    ASSET_TAG_CIRCLE = 2,
    ASSET_TAG_DEBUG_OVERLAY = 3,
    ASSET_TAG_DEMON = 4,
    ASSET_TAG_ELF = 5,
    ASSET_TAG_FLOOR = 6,
    ASSET_TAG_FORK = 7,
    ASSET_TAG_ITEM_HOLDER = 8,
    ASSET_TAG_KNIFE = 9,
    ASSET_TAG_MOUSE_POINTER = 10,
    ASSET_TAG_POTION = 11,
    ASSET_TAG_SHILED = 12,
    ASSET_TAG_STATS_BACKGROUND = 13,
    ASSET_TAG_STATS_UI = 14,
    ASSET_TAG_SWORD = 15,
    ASSET_TAG_WALL = 16,
    ASSET_TAG_ZOMBIE = 17,
    ASSET_TAG_COUNT
};


enum AssetAttributeTag
{
    ASSET_ATTRIB_COLOR = 0,
    ASSET_ATTRIB_DIRECTION = 1,
    ASSET_ATTRIB_SIZE = 2,
    ASSET_ATTRIB_COUNT
};

struct AssetAttributeVector
{
    union
    {
        i32 i[ASSET_ATTRIB_COUNT];
        f32 f[ASSET_ATTRIB_COUNT];
        u8  v[ASSET_ATTRIB_COUNT * 4];
    };
};
