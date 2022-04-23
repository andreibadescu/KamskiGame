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
    ComponentVector<FollowComponent>& followers = entityRegistry.getComponentVector<FollowComponent>();
    ComponentVector<SpriteComponent>& sprites = entityRegistry.getComponentVector<SpriteComponent>();
    ComponentVector<SolidColorComponent>& solidColors = entityRegistry.getComponentVector<SolidColorComponent>();

    for (const Entity followerId: followers.iterateEntities())
    {
        const FollowComponent& follower = followers.getComponent(followerId);
        glm::vec2 toFollowPosition = {};

        if (entityRegistry.hasComponent<SpriteComponent>(follower.toFollowId))
        {
            toFollowPosition = sprites.getComponent(follower.toFollowId).position;
        }
        else if (entityRegistry.hasComponent<SolidColorComponent>(follower.toFollowId))
        {
            toFollowPosition = solidColors.getComponent(follower.toFollowId).position;
        }
        else
        {
            entityRegistry.markEntityForDeletion(followerId);
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
    entityRegistry.removeMarkedEntities();
}

void GameState::updateHealthBars()
{
    for (const Entity healthBarId:
         entityRegistry.iterateEntities<HealthBarComponent,SolidColorComponent, FollowComponent>())
    {
        HealthBarComponent& healthBar = entityRegistry.getComponent<HealthBarComponent>(healthBarId);
        SolidColorComponent& solidColor = entityRegistry.getComponent<SolidColorComponent>(healthBarId);
        FollowComponent& follower = entityRegistry.getComponent<FollowComponent>(healthBarId);

        const f32 healthPoints = entityRegistry.getComponent<EntityComponent>(follower.toFollowId).healthPoints;

        solidColor.size.x = healthBar.maxSize.x * healthPoints / healthBar.maxHealth;
    }
}

glm::vec3 GameState::getCamera() const
{
    return camera;
}

void GameState::addPlayer(const glm::vec2 position, const TextureTag tag,
                          const f32 movementSpeed, const f32 healthPoints, const f32 attackPoints)
{

    const Entity eId = entityRegistry.createEntity();
    playerEId = eId;
    
    camera.z = 1;
    camera.x = position.x;
    camera.y = position.y;

    entityRegistry.addComponent<SpriteComponent>(eId, position, TEXTURE_SIZES[static_cast<u32>(tag)], getTextureIdByTag(tag));
    entityRegistry.addComponent<EntityComponent>(eId, movementSpeed, healthPoints, attackPoints);
    entityRegistry.addComponent<TagComponent>(eId, TagComponent::PLAYER);
    Entity healthbarEId = entityRegistry.createEntity();

    entityRegistry.addComponent<SolidColorComponent>(healthbarEId, SolidColorComponent{{0, 0}, {TEXTURE_SIZES[static_cast<u32>(tag)].x, 20.0f}, {0.0f, 1.0f, 0.0f, 1.0f}});
    entityRegistry.addComponent<FollowComponent>(healthbarEId, FollowComponent{ playerEId, {0, TEXTURE_SIZES[static_cast<u32>(tag)].y / 3.5f}});
    entityRegistry.addComponent<HealthBarComponent>(healthbarEId, HealthBarComponent{{TEXTURE_SIZES[static_cast<u32>(tag)].x, 50.0f}, healthPoints});
}

void GameState::addEnemy(const glm::vec2 position, const TextureTag tag,
                         const f32 movementSpeed, const f32 healthPoints, const f32 attackPoints)
{
    const Entity eId = entityRegistry.createEntity();

    entityRegistry.addComponent<SpriteComponent>(eId, position, TEXTURE_SIZES[static_cast<u32>(tag)], getTextureIdByTag(tag));
    entityRegistry.addComponent<EntityComponent>(eId, movementSpeed, healthPoints, attackPoints);
    entityRegistry.addComponent<TagComponent>(eId, TagComponent::ENEMY);

    const Entity healthBarId = entityRegistry.createEntity();

    entityRegistry.addComponent<SolidColorComponent>(healthBarId, SolidColorComponent{{0, 0}, {TEXTURE_SIZES[static_cast<u32>(tag)].x, 20.0f}, {1.0f, 0.0f, 0.0f, 1.0f}});
    entityRegistry.addComponent<FollowComponent>(healthBarId, FollowComponent{eId, {0, TEXTURE_SIZES[static_cast<u32>(tag)].y / 2.0f}});
    entityRegistry.addComponent<HealthBarComponent>(healthBarId, HealthBarComponent{{TEXTURE_SIZES[static_cast<u32>(tag)].x, 50.0f}, healthPoints});
}

void GameState::updatePlayerPosition()
{
    SpriteComponent& playerSprite = entityRegistry.getComponent<SpriteComponent>(playerEId);
    EntityComponent& playerEntity = entityRegistry.getComponent<EntityComponent>(playerEId);

    if (actionState.zoomIn == KeyState::HOLD)
    {
        camera.z += deltaTime;
    }

    if (actionState.zoomOut == KeyState::HOLD)
    {
        camera.z -= deltaTime;
    }

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

    playerSprite.position.x = max(playerSprite.position.x, -SCREEN_SIZE_WORLD_COORDS + map.getQuadSize().x * 1.7f);
    playerSprite.position.x = min(playerSprite.position.x, SCREEN_SIZE_WORLD_COORDS - map.getQuadSize().x * 1.7f);
    playerSprite.position.y = max(playerSprite.position.y, -SCREEN_SIZE_WORLD_COORDS + map.getQuadSize().y * 2.1f);
    playerSprite.position.y = min(playerSprite.position.y, SCREEN_SIZE_WORLD_COORDS - map.getQuadSize().y * 1.5f);
    
    camera.x = playerSprite.position.x;
    camera.y = playerSprite.position.y;

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
    const SpriteComponent& playerSprite = entityRegistry.getComponent<SpriteComponent>(playerEId);
    EntityComponent& playerEntity = entityRegistry.getComponent<EntityComponent>(playerEId);

    for (const auto& enemyId: entityRegistry.iterateEntities<TagComponent, SpriteComponent, EntityComponent>())
    {
        if (entityRegistry.getComponent<TagComponent>(enemyId) == TagComponent::PLAYER)
            continue;
        const SpriteComponent& enemySprite = entityRegistry.getComponent<SpriteComponent>(enemyId);
        const EntityComponent& enemyEntity = entityRegistry.getComponent<EntityComponent>(enemyId);

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
    if (actionState.attack != KeyState::PRESS && actionState.attack != KeyState::HOLD)
    {
        return;
    }

    const Entity projectileId = entityRegistry.createEntity();
    
    // cursorPosition is not affected by the camera position in calculations.
    //To convert cursorPosition to actual world Position you have to add the camera position to it
    const glm::vec2 directionVector = glm::normalize(cursorPosition + glm::vec2{camera.x, camera.y} - entityRegistry.getComponent<SpriteComponent>(playerEId).position);
    entityRegistry.addComponent<ProjectileComponent>(projectileId, directionVector, 400.0f, playerEId);
    entityRegistry.addComponent<SpriteComponent>(projectileId,
                                                 entityRegistry.getComponent<SpriteComponent>(playerEId).position,
                                                 TEXTURE_SIZES[static_cast<u32>(TextureTag::PROJECTILE)],
                                                 getTextureIdByTag(TextureTag::PROJECTILE),
                                                 atan2(directionVector.y, directionVector.x) - PI / 2.0f);
}

void GameState::updatePlayer()
{
    updatePlayerPosition();
    updatePlayerAttack();
    updatePlayerHealth();
}

void GameState::initECS()
{
    entityRegistry.initEntityQueue();
}

EntityRegistry<ComponentList<KAMSKI_COMPONENTS>>& GameState::getECS()
{
    return entityRegistry;
}

/* Custom collision */
void GameState::updateEnemies()
{
    const SpriteComponent& playerSprite = entityRegistry.getComponent<SpriteComponent>(playerEId);

    for (const auto& enemyEntityId: entityRegistry.iterateEntities<SpriteComponent, EntityComponent, TagComponent>())
    {
        if (entityRegistry.getComponent<TagComponent>(enemyEntityId) == TagComponent::PLAYER)
        {
            continue;
        }
        SpriteComponent& enemySprite = entityRegistry.getComponent<SpriteComponent>(enemyEntityId);
        EntityComponent& enemyEntity = entityRegistry.getComponent<EntityComponent>(enemyEntityId);
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

        for (const auto& otherEnemeyEntityId: entityRegistry.iterateEntities<SpriteComponent, EntityComponent, TagComponent>())
        {

            if (otherEnemeyEntityId == enemyEntityId ||
                entityRegistry.getComponent<TagComponent>(otherEnemeyEntityId) == TagComponent::PLAYER)
            {
                continue;
            }

            const SpriteComponent& otherEnemySprite = entityRegistry.getComponent<SpriteComponent>(otherEnemeyEntityId);

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
    const ComponentVector<SpriteComponent>& sprites = entityRegistry.getComponentVector<SpriteComponent>();
    Entity* const sortedEntityIdsArray = static_cast<Entity*>(MEMORY->temporaryAlloc(sprites.size() * sizeof(Entity)));

    for (const Entity entityId: sprites.iterateEntities())
    {
        sortedEntityIdsArray[cnt++] = entityId;
    }

    std::sort(sortedEntityIdsArray, sortedEntityIdsArray + cnt, [this](const Entity A, const Entity B)
    {
        const f32 aY = entityRegistry.getComponent<SpriteComponent>(A).position.y;
        const f32 bY = entityRegistry.getComponent<SpriteComponent>(B).position.y;
        return aY > bY;
    });

    const SpriteComponent& playerSprite = sprites.getComponent(playerEId);
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
        RENDERER->drawText(glm::vec3{sprite.position, 0.1} + glm::vec3{0, 100, 0}, 320, "GOLEM");
        RENDERER->drawTexturedQuad({ sprite.position, 0.1 }, sprite.size, sprite.textureId, sprite.rotation);
    }

    // Uncomment this if you want the player to be rendered over every other sprite
    //const SpriteComponent playerSprite = sprites.getComponent(PLAYER_ID);
    //draw({ playerSprite.x, playerSprite.y }, map.getQuadSize() *= 2, playerSprite.textureId);

    for (const SolidColorComponent& solidColor: entityRegistry.iterateComponents<SolidColorComponent>())
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
    for (const auto& projectileId : entityRegistry.iterateEntities<SpriteComponent, ProjectileComponent>())
    {
        SpriteComponent& projectileSprite = entityRegistry.getComponent<SpriteComponent>(projectileId);
        ProjectileComponent& projectile = entityRegistry.getComponent<ProjectileComponent>(projectileId);
        projectileSprite.position += projectile.direction * projectile.speed * deltaTime;

        if (projectileSprite.position.x <= -SCREEN_SIZE_WORLD_COORDS || projectileSprite.position.x >= SCREEN_SIZE_WORLD_COORDS ||
            projectileSprite.position.y <= -SCREEN_SIZE_WORLD_COORDS || projectileSprite.position.y >= SCREEN_SIZE_WORLD_COORDS )
        {
            entityRegistry.markEntityForDeletion(projectileId);
            continue;
        }

        for (const auto& enemyId: entityRegistry.iterateEntities<SpriteComponent, EntityComponent, TagComponent>())
        {
            if (entityRegistry.getComponent<TagComponent>(enemyId) == entityRegistry.getComponent<TagComponent>(projectile.shooterId))
                continue;

            const SpriteComponent& enemySprite = entityRegistry.getComponent<SpriteComponent>(enemyId);
            const f32 distanceBetween = glm::distance(enemySprite.position, projectileSprite.position);
            if (distanceBetween <= min(enemySprite.size.x, enemySprite.size.y) / 2)
            //if (isCollision(enemySprite, projectileSprite))
            {
                const Entity shooterId = entityRegistry.getComponent<ProjectileComponent>(projectileId).shooterId;
                // subtract shooter's attack from enemy's health
                entityRegistry.getComponent<EntityComponent>(enemyId).healthPoints -=
                    entityRegistry.getComponent<EntityComponent>(shooterId).attackPoints;
                if (entityRegistry.getComponent<EntityComponent>(enemyId).healthPoints <= 0.0f)
                {
                    entityRegistry.markEntityForDeletion(enemyId);
                }
                // delete projectile
                entityRegistry.markEntityForDeletion(projectileId);
            }
        }
    }
    entityRegistry.removeMarkedEntities();
}

bool GameState::gameHasStarted() const
{
    return !gameOver;
}
