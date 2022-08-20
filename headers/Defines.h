#pragma once

#include <KamskiEngine/engine/deps/glm/glm.hpp>
#include "KamskiEngine/KamskiApi.h"

//TODO: get rid of classes
// make the code more imperative/functional where it makes sense

struct GameState;
inline GameState* gameState;

using TextureId = u32;
using ItemBit = u32;

//TODO: create a math library
inline constexpr f32 PI = 3.14159265f;

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

struct ItemSet
{
    u32 weapons;
    u32 armours;
    u32 utility;
};

struct ActionStates
{
    KeyState startGame;
    KeyState walkUp;
    KeyState walkDown;
    KeyState walkLeft;
    KeyState walkRight;
    KeyState attack;
    KeyState restart;
    KeyState zoomIn;
    KeyState zoomOut;
    KeyState pauseGame;
    KeyState menuInteract;
    KeyState hp;
    KeyState stam;
    KeyState mana;
    KeyState fastRestart;
};

#define KASMKI_MAX_ENTITY_COUNT 20000
