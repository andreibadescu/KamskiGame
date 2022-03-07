#include "headers/GameState.h"
#include "KamskiEngine/engine/deps/glm/glm.hpp"

void GameState::linkTextureIdByTag(const texture_id id, const TextureTag tag)
{
    textureIdsByTag[static_cast<u32>(tag)] = id;
}

texture_id GameState::getTextureIdByTag(const TextureTag tag)
{
    return textureIdsByTag[static_cast<u32>(tag)];
}

void GameState::setDrawFunction(void(*renderFunction)(const glm::vec2& position, const glm::vec2& size, texture_id texId))
{
    draw = renderFunction;
}

void GameState::addPlayer(const Entity eId, const f32 x, const f32 y, const texture_id textureId,
                          const f32 movementSpeed, const u32 healthPoints, const u32 attackPoints)
{
    sprites.addComponent(eId, x, y, textureId);
    players.addComponent(eId, movementSpeed, healthPoints, attackPoints);
}

void GameState::addEnemy(const Entity eId, const f32 x, const f32 y, const texture_id textureId,
                         const f32 movementSpeed, const u32 healthPoints, const u32 attackPoints)
{
    sprites.addComponent(eId, x, y, textureId);
    enemies.addComponent(eId, movementSpeed, healthPoints, attackPoints);
}

void GameState::updatePlayer(const f32 deltaTime)
{
    SpriteComponent& playerSprite = sprites.getComponent(PLAYER_ID);
    EntityComponent& playerEntity = players.getComponent(PLAYER_ID);
    const f32 distance = deltaTime * playerEntity.movementSpeed;
    const bool upXorDown = input.isUpKeyPressed() ^ input.isDownKeyPressed();
    const bool leftXorRight = input.isLeftKeyPressed() ^ input.isRightKeyPressed();
    if (upXorDown) {
        if (input.isUpKeyPressed())
        {
            if (leftXorRight)
            {
                playerSprite.y += distance / glm::sqrt(2.0f);
            }
            else
            {
                playerSprite.y += distance;
            }
        }
        else if (input.isDownKeyPressed())
        {
            if (leftXorRight)
            {
                playerSprite.y -= distance / glm::sqrt(2.0f);
            }
            else
            {
                playerSprite.y -= distance;
            }
        }
    }
    if (leftXorRight) {
        if (input.isLeftKeyPressed())
        {
            if (upXorDown)
            {
                playerSprite.x -= distance / glm::sqrt(2.0f);
            }
            else
            {
                playerSprite.x -= distance;
            }
        }
        else if (input.isRightKeyPressed())
        {
            if (upXorDown)
            {
                playerSprite.x += distance / glm::sqrt(2.0f);
            }
            else
            {
                playerSprite.x += distance;
            }
        }
    }
    playerSprite.x = std::max(playerSprite.x, -1.0f + map.getQuadSize().x * 2.0f);
    playerSprite.x = std::min(playerSprite.x, 1.0f - map.getQuadSize().x * 2.0f);
    playerSprite.y = std::max(playerSprite.y, -1.0f + map.getQuadSize().y * 1.9f);
    playerSprite.y = std::min(playerSprite.y, 1.0f - map.getQuadSize().y * 1.5f);
}

void GameState::updateEnemies(const f32 deltaTime)
{
    SpriteComponent& playerSprite = sprites.getComponent(PLAYER_ID);
    for (const auto& enemyEntityId: enemies.iterateEntities())
    {
        SpriteComponent& enemySprite = sprites.getComponent(enemyEntityId);
        EntityComponent& enemyEntity = enemies.getComponent(enemyEntityId);
        const glm::vec2 enemyVector{ enemySprite.x - playerSprite.x, enemySprite.y - playerSprite.y };
        const glm::vec2 normalizedEnemyVector = normalize(enemyVector);
        const f32 distance = deltaTime * enemyEntity.movementSpeed;

        const glm::vec2 newPosition{
            enemySprite.x - normalizedEnemyVector.x * distance,
            enemySprite.y - normalizedEnemyVector.y * distance
        };
        // detect collision
        bool isCollision = false;
        for (const auto& otherEnemeyEntityId: enemies.iterateEntities())
        {
            if (otherEnemeyEntityId == enemyEntityId)
            {
                continue;
            }
            const SpriteComponent& otherEnemySprite = sprites.getComponent(otherEnemeyEntityId);
            const glm::vec2 vectorBetween{ otherEnemySprite.x - newPosition.x, otherEnemySprite.y - newPosition.y };
            const f32 distanceBetween = glm::sqrt(vectorBetween.x * vectorBetween.x + vectorBetween.y * vectorBetween.y);
            if (distanceBetween <= map.getQuadSize().x)
            {
                isCollision = true;
                break;
            }
        }

        // collision with player
        const glm::vec2 vectorBetween{ playerSprite.x - newPosition.x, playerSprite.y - newPosition.y };
        const f32 distanceBetween = glm::sqrt(vectorBetween.x * vectorBetween.x + vectorBetween.y * vectorBetween.y);
        if (distanceBetween <= map.getQuadSize().x * 1.5f)
        {
            isCollision = true;
        }

        if (!isCollision)
        {
            enemySprite.x = newPosition.x;
            enemySprite.y = newPosition.y;
        }
    }
}

void GameState::renderSprites() const
{
    for (const SpriteComponent sprite: sprites.iterateComponents())
    {
        const glm::vec2 position{ sprite.x, sprite.y };
        draw(position, map.getQuadSize() *= 2, sprite.textureId);
    }
    // Uncomment this if you want the player to be rendered over every other sprite
    //const SpriteComponent playerSprite = sprites.getComponent(PLAYER_ID);
    //draw({ playerSprite.x, playerSprite.y }, map.getQuadSize() *= 2, playerSprite.textureId);
}
