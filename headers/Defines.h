#pragma once

#include <KamskiEngine/engine/deps/glm/glm.hpp>
#include "KamskiEngine/KamskiApi.h"

//TODO: get rid of classes
// make the code more imperative/functional where it makes sense

class Game;
inline Game* GAME;

using TextureId = u32;
using ItemBit = u32;

// TODO: hot-reload all of these constants from a config file
// bake these in, in a release build
inline constexpr u32 MAP_SIZE = 300;
inline constexpr u32 MAP_SIZE_Y = MAP_SIZE;
inline constexpr u32 MAP_SIZE_X = MAP_SIZE;

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
inline constexpr f32 ENEMY_DETECTION_RADIUS = 180.0f;
inline constexpr f32 QUAD_SIZE = 16.0f;
inline constexpr f32 HEALTH_BAR_HEIGHT = 2.0f;
inline constexpr f32 HEALTH_BAR_HEIGHT_OFFSET = 5.0f;
inline constexpr f32 ANIMATIONS_MULTIPLIER = 1.0f;
inline constexpr f32 ENTITIES_SIZE_MULTIPLIER = 1.0f;

//TODO: create a math library
inline constexpr f32 PI = 3.14159265f;


/* ENUMS START */
enum class AssetTag: u32
{
    NONE,
    CIRCLE,
    DEBUG_OVERLAY,
    POTION,
    BUTTON,
    MOUSE_POINTER,
    STATS_UI,
    ITEM_HOLDER,
    STATS_BACKGROUND,
    SWORD,
    SHILED,
    FORK,
    KNIFE,
    FLOOR,
    WALL,
    ELF,
    DEMON,
    ZOMBIE,
    CHEST,
    COUNT
};

enum EntityType: u32
{
    ELF,
    DEMON,
    ZOMBIE,
    ENTITY_TYPE_COUNT,
};

enum AnimationState: u32
{
    IDLE,
    RUN,
    HIT,
    SPECIAL_0,
    ANIMATION_STATE_COUNT
};

enum ItemType: u32
{
    ITEM_TYPE_WEAPON,
    ITEM_TYPE_ARMOUR,
    ITEM_TYPE_UTILITY
};

enum Weapon: u32
{
    WEAPON_SWORD,
    WEAPON_SHIELD,
    WEAPON_FORK,
    WEAPON_KNIFE,
    WEAPON_COUNT
};

enum Armour: u32
{
    ARMOUR_HELMET,
    ARMOUR_CHESTPLATE,
    ARMOUR_PANTS,
    ARMOUR_COUNT
};

enum Utility: u32
{
    UTILITY_GRENADE,
    UTILITY_POTION,
    UTILITY_ROPE,
    UTILITY_COUNT
};

struct EntityStats
{
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

enum State 
{
    GAME_STATE_MAIN_MENU,
    GAME_STATE_SETTINGS,
    GAME_STATE_START,
    GAME_STATE_RUNNING,
    GAME_STATE_PAUSED,
    GAME_STATE_LOST,
    GAME_STATE_COUNT
};

#define KASMKI_MAX_ENTITY_COUNT 20000
#define KAMSKI_COMPONENTS TransformComponent, TypeComponent, ColliderComponent, SpriteComponent, SolidColorComponent, FollowComponent, EntityComponent, ProjectileComponent, HealthBarComponent, ItemComponent, VelocityComponent, EnemyComponent
