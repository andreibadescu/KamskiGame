#pragma once

#include "KamskiEngine/KamskiContainers.h"

#include "Map.h"
#include "Components.h"
#include "InputState.h"

class GameState
{
public:
    Map map;
    InputState input;

    GameState() = delete;

    static void linkTextureIdByTag(texture_id id, TextureTag tag);

    [[nodiscard]]
    static texture_id getTextureIdByTag(TextureTag tag);

    void setDrawFunction(void(*)(const glm::vec2& position, const glm::vec2& size, texture_id texId));

    void addPlayer(Entity eId, f32 x, f32 y, texture_id textureId, f32 movementSpeed, u32 healthPoints, u32 attackPoints);

    void addEnemy(Entity eId, f32 x, f32 y, texture_id textureId, f32 movementSpeed, u32 healthPoints, u32 attackPoints);

    void updatePlayer(f32 deltaTime);

    void updateEnemies(f32 deltaTime);

    void renderSprites() const;

private:
    inline static texture_id textureIdsByTag[static_cast<u32>(TextureTag::COUNT)]{};

    ComponentVector<SpriteComponent> sprites;
    ComponentVector<EntityComponent> players;
    ComponentVector<EntityComponent> enemies;

    void(*draw)(const glm::vec2& position, const glm::vec2& size, texture_id texId);
};
