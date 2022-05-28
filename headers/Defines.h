#pragma once

#include <KamskiEngine/engine/deps/glm/glm.hpp>
#include "KamskiEngine/KamskiApi.h"

class Game;
inline Game* GAME;

using texture_id = u32;

inline constexpr u32 MAP_SIZE = 300;
inline constexpr u32 MAP_SIZE_Y = MAP_SIZE;
inline constexpr u32 MAP_SIZE_X = MAP_SIZE;
// transform this to constexpr
inline struct {
    glm::uvec2 MIN{20, 20};
    glm::uvec2 MAX{30, 30};
} ROOM;
inline constexpr u32 MAX_WALLS = MAP_SIZE / 20 * 7;
#ifdef KAMSKI_DEBUG
inline constexpr f32 DEFAULT_CAMERA_ZOOM = 3.0f;
#else
inline constexpr f32 DEFAULT_CAMERA_ZOOM = 7.0f;
#endif
inline constexpr f32 DEFAULT_PLAYER_SPEED = 75.0f;
inline constexpr f32 DEFAULT_ENEMY_SPEED = 40.0f;
inline constexpr f32 ENEMY_DETECTION_RADIUS = 150.0f;
inline constexpr f32 QUAD_SIZE = 16.0f;
inline constexpr f32 HEALTH_BAR_HEIGHT = 2.0f;
inline constexpr f32 HEALTH_BAR_HEIGHT_OFFSET = 5.0f;
//TODO: create a math library
inline constexpr f32 PI = 3.14159265f;

enum class TextureTag: u8
{
    NONE,
    ENEMY,
    PLAYER,
    PROJECTILE,
    CIRCLE,
    BUTTON,
    BUTTON_PRESSED,
    DEBUG_OVERLAY,
    /* MAP TILES, RATIO ALWAYS 1:1 SO ADD THEM ONLY AFTER THIS COMMENT */
    FLOOR, // COLLISION_START
    WALL,
    WALL_BOTTOM,
    WALL_TOP,
    WALL_LEFT,
    WALL_RIGHT,
    WALL_CORNER_BOTTOM_LEFT,
    WALL_CORNER_BOTTOM_RIGHT,
    WALL_CORNER_TOP_LEFT,
    WALL_CORNER_TOP_RIGHT, // COLLISION_END
    // animations
    BIG_ZOMBIE_IDLE_0,
    BIG_ZOMBIE_IDLE_1,
    BIG_ZOMBIE_IDLE_2,
    BIG_ZOMBIE_IDLE_3,
    BIG_ZOMBIE_RUN_0,
    BIG_ZOMBIE_RUN_1,
    BIG_ZOMBIE_RUN_2,
    BIG_ZOMBIE_RUN_3,
    ELF_M_IDLE_0,
    ELF_M_IDLE_1,
    ELF_M_IDLE_2,
    ELF_M_IDLE_3,
    ELF_M_RUN_0,
    ELF_M_RUN_1,
    ELF_M_RUN_2,
    ELF_M_RUN_3,
    // the end
    COUNT
};

enum AnimationState
{
    IDLE,
    RUN,
    ANIMATION_STATE_COUNT
};

enum EntityType
{
    PLAYER,
    ZOMBIE,
    ENTITY_TYPE_COUNT
};

struct EntityStats
{
    TextureTag tag;
    f32 movementSpeed;
    f32 healthPoints;
    f32 attackPoints;
};

struct ItemSet
{
    u32 weapons;
    u32 armours;
    u32 utility;
};

enum ItemType : u32
{
    ITEM_TYPE_WEAPON,
    ITEM_TYPE_ARMOUR,
    ITEM_TYPE_UTILITY
};

enum Weapon : u32
{
    WEAPON_BOW    = BIT(0),
    WEAPON_SWORD  = BIT(1),
    WEAPON_SHIELD = BIT(2)
};

enum Armour : u32
{
    ARMOUR_HELMET = BIT(0),
    ARMOUR_CHESTPLATE = BIT(1),
    ARMOUR_PANTS = BIT(2)
};

enum Utility : u32
{
    UTILITY_GRENADE = BIT(0),
    UTILITY_POTION = BIT(1),
    UTILITY_ROPE = BIT(2)
};

using ItemBit = u32;

constexpr EntityStats ENEMIES_STATS[] = {
    [PLAYER] = {TextureTag::PLAYER, DEFAULT_PLAYER_SPEED, 200.0f, 50.0f},
    [ZOMBIE] = {TextureTag::ENEMY, DEFAULT_ENEMY_SPEED, 200.0f, 25.0f}
};

enum States {
    MAIN_MENU,
    SETTINGS,
    GAME_START,
    GAME_RUNNING,
    GAME_PAUSED,
    GAME_LOST
};

#define COLLISION_START WALL
#define COLLISION_END WALL_CORNER_TOP_RIGHT

#define TAG(TEXTURE) ((u32)TextureTag::TEXTURE)
constexpr glm::vec2 TEXTURE_SIZES[] = {
    [TAG(ENEMY)]      = {18.0f, 27.0f},
    [TAG(PLAYER)]     = {15.0f, 18.0f},
    [TAG(PROJECTILE)] = {12.0f, 26.0f},
    [TAG(CIRCLE)]     = {10.0f, 10.0f}
};

// TO-DO: UNCOMMENT THIS
// static_assert(ROOM.MIN.x <= ROOM.MAX.x && ROOM.MIN.y <= ROOM.MAX.y);
// #ifndef KAMSKI_DEBUG
// static_assert(ROOM.MIN.x > 9 && ROOM.MIN.y > 9);
// #endif

// concatenating assets' dir with filename

#define TEXTURE_ASSET(filename) ("assets/" filename)
inline const char* TEXTURE_PATHS[] = {
    // [TAG(NONE)]                     = TEXTURE_ASSET("empty.png"),
    [TAG(NONE)]                     = TEXTURE_ASSET("debug.png"),
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
    [TAG(BUTTON_PRESSED)]           = TEXTURE_ASSET("menu_button_press.png"),
    [TAG(DEBUG_OVERLAY)]            = TEXTURE_ASSET("debug_overlay.png")
};

inline constexpr u32 TEXTURE_COUNT = ARRAY_COUNT(TEXTURE_PATHS);

#undef TEXTURE_ASSET
#define KASMKI_MAX_ENTITY_COUNT 20000
#define KAMSKI_COMPONENTS SpriteComponent, TagComponent, SolidColorComponent, FollowComponent, EntityComponent, ProjectileComponent, HealthBarComponent, ItemComponent, VelocityComponent
#define ID(TAG) getTextureIdByTag(TextureTag::TAG)
