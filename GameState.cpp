#include "headers/GameState.h"
#include "KamskiEngine/engine/deps/glm/glm.hpp"
#include <queue>

bool GameState::isCollision(const SpriteComponent& A, const SpriteComponent& B)
{
    return (A.position.x < B.position.x + B.size.x) &&
           (A.position.x + A.size.x > B.position.x) &&
           (A.position.y < B.position.y + B.size.y / 2) &&
           (A.position.y + A.size.y / 2 > B.position.y);
}

bool GameState::playerHasDied() const
{
    return gameOver;
}

void GameState::linkTextureIdByTag(const texture_id id, const TextureTag tag)
{
    textureIdsByTag[static_cast<u32>(tag)] = id;
}

texture_id GameState::getTextureIdByTag(const TextureTag tag) const
{
    return textureIdsByTag[static_cast<u32>(tag)];
}
void GameState::updateFollowers()
{
    Entity* markedForDeletion = static_cast<Entity*>(MEMORY->temporaryAlloc(followers.size() * sizeof(Entity)));
    u32 markedCount = 0;

    for (const Entity followerId: followers.iterateEntities())
    {
        const FollowComponent& follower = followers.getComponent(followerId);
        glm::vec2 toFollowPosition = {};

        if (sprites.hasComponent(follower.toFollowId))
        {
            toFollowPosition = sprites.getComponent(follower.toFollowId).position;
        }
        else if (solidColors.hasComponent(follower.toFollowId))
        {
            toFollowPosition = solidColors.getComponent(follower.toFollowId).position;
        }
        else
        {
            markedForDeletion[markedCount++] = followerId;
            continue;
        }

        if (solidColors.hasComponent(followerId))
        {
            SolidColorComponent& solidColor = solidColors.getComponent(followerId);
            solidColor.position = toFollowPosition + follower.followOffset;
        }
        else
        {
            // Handle following sprites
        }
    }

    for (u32 i = 0; i < markedCount; i++)
    {
        sprites.removeComponent(markedForDeletion[i]);
        enemies.removeComponent(markedForDeletion[i]);
        projectiles.removeComponent(markedForDeletion[i]);
        healthBars.removeComponent(markedForDeletion[i]);
        followers.removeComponent(markedForDeletion[i]);
        solidColors.removeComponent(markedForDeletion[i]);
    }
}

void GameState::updateHealthBars()
{
    for (const Entity healthBarId: healthBars.iterateEntities())
    {
        HealthBarComponent& healthBar = healthBars.getComponent(healthBarId);
        SolidColorComponent& solidColor = solidColors.getComponent(healthBarId);
        FollowComponent& follower = followers.getComponent(healthBarId);
        // TODO: merge players and enemies so that this shit vvv can be general

        f32 healthPoints = 0;
        if (players.hasComponent(follower.toFollowId))
        {
            healthPoints = players.getComponent(follower.toFollowId).healthPoints;
        }
        else if (enemies.hasComponent(follower.toFollowId))
        {
            healthPoints = enemies.getComponent(follower.toFollowId).healthPoints;
        }
        else
        {
            continue;
        }

        solidColor.size.x = healthBar.maxSize.x * healthPoints / healthBar.maxHealth;
    }
}

void GameState::addPlayer(const Entity eId, const glm::vec2 position, const TextureTag tag,
                          const f32 movementSpeed, const f32 healthPoints, const f32 attackPoints)
{
    sprites.addComponent(eId, position, TEXTURE_SIZES[static_cast<u32>(tag)], getTextureIdByTag(tag));
    players.addComponent(eId, movementSpeed, healthPoints, attackPoints);
    solidColors.addComponent(eId + 1, SolidColorComponent{{0, 0}, {TEXTURE_SIZES[static_cast<u32>(tag)].x, 0.02f}, {0.0f, 1.0f, 0.0f, 1.0f}});
    followers.addComponent(eId + 1, FollowComponent{eId, {0, TEXTURE_SIZES[static_cast<u32>(tag)].y / 3.5f}});
    healthBars.addComponent(eId + 1, HealthBarComponent{{TEXTURE_SIZES[static_cast<u32>(tag)].x, 0.05f}, healthPoints});
}

void GameState::addEnemy(const Entity eId, const glm::vec2 position, const TextureTag tag,
                         const f32 movementSpeed, const f32 healthPoints, const f32 attackPoints)
{
    sprites.addComponent(eId, position, TEXTURE_SIZES[static_cast<u32>(tag)], getTextureIdByTag(tag));
    enemies.addComponent(eId, movementSpeed, healthPoints, attackPoints);
    solidColors.addComponent(eId + 1, SolidColorComponent{{0, 0}, {TEXTURE_SIZES[static_cast<u32>(tag)].x, 0.02f}, {1.0f, 0.0f, 0.0f, 1.0f}});
    followers.addComponent(eId + 1, FollowComponent{eId, {0, TEXTURE_SIZES[static_cast<u32>(tag)].y / 2.0f}});
    healthBars.addComponent(eId + 1, HealthBarComponent{{TEXTURE_SIZES[static_cast<u32>(tag)].x, 0.05f}, healthPoints});
}

void GameState::updatePlayerPosition()
{
    SpriteComponent& playerSprite = sprites.getComponent(PLAYER_ID);
    EntityComponent& playerEntity = players.getComponent(PLAYER_ID);

    //const glm::vec2 oldPosition = playerSprite.position;
    const f32 distance = deltaTime * playerEntity.movementSpeed;

    const bool isWalkUp = (actionState.walkUp == KeyState::HOLD || actionState.walkUp == KeyState::PRESS);
    const bool isWalkDown = (actionState.walkDown == KeyState::HOLD || actionState.walkDown == KeyState::PRESS);
    const bool isWalkLeft = (actionState.walkLeft == KeyState::HOLD || actionState.walkLeft == KeyState::PRESS);
    const bool isWalkRight = (actionState.walkRight == KeyState::HOLD || actionState.walkRight == KeyState::PRESS);

    const bool upXorDown = isWalkUp ^ isWalkDown;
    const bool leftXorRight = isWalkLeft ^ isWalkRight;

    if (upXorDown)
    {
        if (isWalkUp)
        {
            if (leftXorRight)
            {
                playerSprite.position.y += distance / glm::sqrt(2.0f);
            }
            else
            {
                playerSprite.position.y += distance;
            }
        }
        else if (isWalkDown)
        {
            if (leftXorRight)
            {
                playerSprite.position.y -= distance / glm::sqrt(2.0f);
            }
            else
            {
                playerSprite.position.y -= distance;
            }
        }
    }
    if (leftXorRight)
    {
        if (isWalkLeft)
        {
            if (upXorDown)
            {
                playerSprite.position.x -= distance / glm::sqrt(2.0f);
            }
            else
            {
                playerSprite.position.x -= distance;
            }
        }
        else if (isWalkRight)
        {
            playerSprite.size.x = abs(playerSprite.size.x);
            if (upXorDown)
            {
                playerSprite.position.x += distance / glm::sqrt(2.0f);
            }
            else
            {
                playerSprite.position.x += distance;
            }
        }
    }

    playerSprite.position.x = max(playerSprite.position.x, -1.0f + map.getQuadSize().x * 1.7f);
    playerSprite.position.x = min(playerSprite.position.x, 1.0f - map.getQuadSize().x * 1.7f);
    playerSprite.position.y = max(playerSprite.position.y, -1.0f + map.getQuadSize().y * 2.1f);
    playerSprite.position.y = min(playerSprite.position.y, 1.0f - map.getQuadSize().y * 1.5f);

    //for (const auto& enemyId : enemies.iterateEntities())
    //{
    //    const SpriteComponent& enemySprite = sprites.getComponent(enemyId);
    //    if (isCollision(playerSprite, enemySprite))
    //    {
    //        playerSprite.position = oldPosition;
    //        break;
    //    }
    //}
}

void GameState::updatePlayerHealth()
{
    const SpriteComponent& playerSprite = sprites.getComponent(PLAYER_ID);
    EntityComponent& playerEntity = players.getComponent(PLAYER_ID);

    for (const auto& enemyId: enemies.iterateEntities())
    {
        const SpriteComponent& enemySprite = sprites.getComponent(enemyId);
        const EntityComponent& enemyEntity = enemies.getComponent(enemyId);

        if (isCollision(playerSprite, enemySprite))
        {
            playerEntity.healthPoints -= enemyEntity.attackPoints * deltaTime;
            if (playerEntity.healthPoints <= 0.0f)
            {
                gameOver = true;
                return;
            }
        }
    }
}

void GameState::updatePlayerAttack()
{
    if (actionState.attack != KeyState::PRESS)
    {
        return;
    }
    static Entity projectileId = 100;
    const glm::vec2 directionVector = glm::normalize(cursorPosition - sprites.getComponent(PLAYER_ID).position);
    projectiles.addComponent(projectileId, directionVector, 0.9f, static_cast<Entity>(PLAYER_ID));
    sprites.addComponent(projectileId,
                         sprites.getComponent(PLAYER_ID).position,
                         TEXTURE_SIZES[static_cast<u32>(TextureTag::PROJECTILE)],
                         getTextureIdByTag(TextureTag::PROJECTILE),
                         atan2(directionVector.y, directionVector.x) - 3.14159f / 2.0f);
    ++projectileId;
}

void GameState::updatePlayer()
{
    updatePlayerPosition();
    updatePlayerAttack();
    updatePlayerHealth();
}

/* Custom collision */
void GameState::updateEnemies()
{
    const SpriteComponent& playerSprite = sprites.getComponent(PLAYER_ID);
    for (const auto& enemyEntityId: enemies.iterateEntities())
    {
        SpriteComponent& enemySprite = sprites.getComponent(enemyEntityId);
        EntityComponent& enemyEntity = enemies.getComponent(enemyEntityId);
        const glm::vec2 enemyVector{
            enemySprite.position.x - playerSprite.position.x,
            enemySprite.position.y - playerSprite.position.y
        };
        const glm::vec2 normalizedEnemyVector = normalize(enemyVector);
        const f32 distance = deltaTime * enemyEntity.movementSpeed;

        const glm::vec2 newPosition{
            enemySprite.position.x - normalizedEnemyVector.x * distance,
            enemySprite.position.y - normalizedEnemyVector.y * distance
        };
        // detect collision
        bool bCollision = false;
        for (const auto& otherEnemeyEntityId: enemies.iterateEntities())
        {
            if (otherEnemeyEntityId == enemyEntityId)
            {
                continue;
            }
            const SpriteComponent& otherEnemySprite = sprites.getComponent(otherEnemeyEntityId);
            const glm::vec2 vectorBetween{
                otherEnemySprite.position.x - newPosition.x,
                otherEnemySprite.position.y - newPosition.y
            };
            const f32 distanceBetween = glm::sqrt(vectorBetween.x * vectorBetween.x + vectorBetween.y * vectorBetween.y);
            if (distanceBetween <= map.getQuadSize().x)
            {
                bCollision = true;
                break;
            }
        }

        // collision with player
        const glm::vec2 vectorBetween{
            playerSprite.position.x - newPosition.x,
            playerSprite.position.y - newPosition.y
        };
        const f32 distanceBetween = glm::sqrt(vectorBetween.x * vectorBetween.x + vectorBetween.y * vectorBetween.y);
        if (distanceBetween <= map.getQuadSize().x * 1.5f)
        {
            bCollision = true;
        }

        if (!bCollision)
        {
            enemySprite.position.x = newPosition.x;
            enemySprite.position.y = newPosition.y;
        }
    }
}

/* AABB collision */
//void GameState::updateEnemies()
//{
//    SpriteComponent& playerSprite = sprites.getComponent(PLAYER_ID);
//    for (const auto& enemyEntityId : enemies.iterateEntities())
//    {
//        SpriteComponent& enemySprite = sprites.getComponent(enemyEntityId);
//        EntityComponent& enemyEntity = enemies.getComponent(enemyEntityId);
//
//        const glm::vec2 enemyVector{
//            enemySprite.position.x - playerSprite.position.x,
//            enemySprite.position.y - playerSprite.position.y
//        };
//
//        const glm::vec2 normalizedEnemyVector = normalize(enemyVector);
//        const f32 distance = deltaTime * enemyEntity.movementSpeed;
//
//        const glm::vec2 newPosition{
//            enemySprite.position.x - normalizedEnemyVector.x * distance,
//            enemySprite.position.y - normalizedEnemyVector.y * distance
//        };
//
//        SpriteComponent newEnemySprite{ newPosition, enemySprite.size, enemySprite.textureId };
//
//        // detect collision
//        bool collision = false;
//        for (const auto& otherEnemeyEntityId : enemies.iterateEntities())
//        {
//            if (otherEnemeyEntityId == enemyEntityId)
//            {
//                continue;
//            }
//            const SpriteComponent& otherEnemySprite = sprites.getComponent(otherEnemeyEntityId);
//            if (isCollision(newEnemySprite, otherEnemySprite))
//            {
//                collision = true;
//                break;
//            }
//        }
//
//        if (isCollision(newEnemySprite, playerSprite))
//        {
//            collision = true;
//        }
//
//        if (!collision)
//        {
//            enemySprite.position = newEnemySprite.position;
//        }
//    }
//}

void GameState::renderSprites() const
{
    u32 cnt = 0;
    Entity* const sortedEntityIdsArray = static_cast<Entity*>(MEMORY->temporaryAlloc(sprites.size() * sizeof(Entity)));
    for (const Entity entityId: sprites.iterateEntities())
    {
        sortedEntityIdsArray[cnt++] = entityId;
    }

    std::sort(sortedEntityIdsArray, sortedEntityIdsArray + cnt, [this](const auto A, const auto B)
    {
        const f32 aY = sprites.getComponent(A).position.y;
        const f32 bY = sprites.getComponent(B).position.y;
        return aY > bY;
    });

    const SpriteComponent& playerSprite = sprites.getComponent(PLAYER_ID);
    for (u32 i = 0; i < cnt; ++i)
    {
        SpriteComponent sprite = sprites.getComponent(sortedEntityIdsArray[i]);
        if (sprite.textureId == getTextureIdByTag(TextureTag::PLAYER))
        {
            if (cursorPosition.x < sprite.position.x)
            {
                sprite.size.x = -abs(sprite.size.x);
            }
            else
            {
                sprite.size.x = abs(sprite.size.x);
            }
        }
        else
        {
            if (playerSprite.position.x < sprite.position.x)
            {
                sprite.size.x = -abs(sprite.size.x);
            }
            else
            {
                sprite.size.x = abs(sprite.size.x);
            }
        }
        RENDERER->drawTexturedQuad({ sprite.position, sprite.position.y }, sprite.size, sprite.textureId, sprite.rotation);
    }

    // Uncomment this if you want the player to be rendered over every other sprite
    //const SpriteComponent playerSprite = sprites.getComponent(PLAYER_ID);
    //draw({ playerSprite.x, playerSprite.y }, map.getQuadSize() *= 2, playerSprite.textureId);

    for (const SolidColorComponent solidColor: solidColors.iterateComponents())
    {
        RENDERER->drawColoredQuad({ solidColor.position, solidColor.position.y }, solidColor.size, solidColor.color, solidColor.rotation);
    }
}

void GameState::startGame()
{
    gameOver = false;
}

void GameState::stopGame()
{
    gameOver = true;
}

void GameState::updateDeltaTime(const f32 deltaTime)
{
    this->deltaTime = deltaTime;
}

void GameState::moveProjectiles()
{
    // TODO (phillip): make a Mark Entity for Deletion system
    // Problema era ca stergeai din vector in timp ce-l iterai
    // iar asta nu iese bine niciodata
    // quick fix:
    Entity* markedForDeletion =
        static_cast<Entity*>(MEMORY->temporaryAlloc((enemies.size() + projectiles.size()) * sizeof(Entity)));
    u32 markedCount = 0;

    for (const auto& projectileId : projectiles.iterateEntities())
    {
        SpriteComponent& projectileSprite = sprites.getComponent(projectileId);
        ProjectileComponent& projectile = projectiles.getComponent(projectileId);
        projectileSprite.position += projectile.direction * projectile.speed * deltaTime;
        if (projectileSprite.position.x <= -1.0f || projectileSprite.position.x >= 1.0f ||
            projectileSprite.position.y <= -1.0f || projectileSprite.position.y >= 1.0f)
        {
            markedForDeletion[markedCount++] = projectileId;
            continue;
        }
        for (const auto& enemyId: enemies.iterateEntities())
        {
            const SpriteComponent& enemySprite = sprites.getComponent(enemyId);
            const f32 distanceBetween = glm::distance(enemySprite.position, projectileSprite.position);
            if (distanceBetween <= min(enemySprite.size.x, enemySprite.size.y) / 2)
            //if (isCollision(enemySprite, projectileSprite))
            {
                // subtract shooter's attack from enemy's health
                enemies.getComponent(enemyId).healthPoints -=
                    players.getComponent(projectiles.getComponent(projectileId).shooterId).attackPoints;
                if (enemies.getComponent(enemyId).healthPoints <= 0.0f)
                {
                    markedForDeletion[markedCount++] = enemyId;
                    logInfo("Enemy was killed! - %d", enemyId);
                }
                // delete projectile
                markedForDeletion[markedCount++] = projectileId;
            }
        }
    }

    for (u32 i = 0; i < markedCount; i++)
    {
        sprites.removeComponent(markedForDeletion[i]);
        enemies.removeComponent(markedForDeletion[i]);
        projectiles.removeComponent(markedForDeletion[i]);
        healthBars.removeComponent(markedForDeletion[i]);
        followers.removeComponent(markedForDeletion[i]);
        solidColors.removeComponent(markedForDeletion[i]);
    }
}

bool GameState::gameHasStarted() const
{
    return !gameOver;
}
