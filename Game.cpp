#include "headers/Defines.h"
#include "headers/Components.h"
#include <random>

class Game {
    public:
    struct Map {
        texture_id* tiles;
        // refactor this tilesArr to be cache friendly by also storing the texture id
        // and change uvec2 to vec2 (store the real position, not the relativ position inside the tiles matrix)
        glm::uvec2* tilesArr;
        glm::uvec2 size;
        union Wall {
            struct {
                glm::vec2 leftBottom;
                glm::vec2 topRight;
            };
            glm::vec4 corners;
        }* walls;
        u32 numberOfWalls;
        glm::vec2 quadSize;
        u32 tilesArrSize;
    };

    // MEMORY THAT SHOULDN'T BE CHANGED
    texture_id textureIdsByTag[(u32)TextureTag::COUNT];

    // MEMORY THAT YOU CAN RESET
    union {
        struct {
            f32 deltaTime;
            Entity playerEId;
            EntityRegistry<ComponentList<KAMSKI_COMPONENTS>> entityRegistry;
            glm::vec3 camera;
            bool isVroomOn;
            glm::vec2 startPosition;
            f32 playerRadius;
            animation_id* animations;
            States gameState;
            ItemSet itemSet;
            glm::vec2 buttonSize;
            f32 playerAttackTimer;

            struct
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
            } actionState;

            glm::vec2 cursorPosition;
            Map map;
        };
        char disposableMemory[];
    };

    Game() = delete;

    void gameInit();

    void gameInput();

    void gameUpdate();

    void gameRender();

    bool isCollision(const SpriteComponent& A, const SpriteComponent& B)
    {
        return (abs(A.position.x - B.position.x) <= A.size.x / 2.0f + B.size.x / 2.0f) &&
        (abs(A.position.y - B.position.y) <= A.size.y / 2.0f + B.size.y / 2.0f);
    }

    void linkTextureIdByTag(const texture_id id, const TextureTag tag)
    {
        textureIdsByTag[static_cast<u32>(tag)] = id;
    }

    texture_id getTextureIdByTag(const TextureTag tag) const
    {
        return textureIdsByTag[static_cast<u32>(tag)];
    }

    void updateFollowers()
    {
        ComponentVector<FollowComponent>& followers = entityRegistry.getComponentVector<FollowComponent>();
        ComponentVector<SpriteComponent>& sprites = entityRegistry.getComponentVector<SpriteComponent>();
        ComponentVector<SolidColorComponent>& solidColors = entityRegistry.getComponentVector<SolidColorComponent>();

        for (const Entity followerId: followers.iterateEntities())
        {
            const FollowComponent& follower = followers.getComponent(followerId);
            glm::vec2 toFollowPosition{};

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

    void updateHealthBars()
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

    texture_id getTextureIdByItem(ItemType type, ItemBit bit)
    {
        TextureTag tag = TextureTag::NONE;
        switch (type)
        {
            case ITEM_TYPE_WEAPON:
            {
                tag = (TextureTag)((u32)TextureTag::WEAPON_START + bit);
            }break;
        }

        return getTextureIdByTag(tag);
    }

    const char* getItemDescription(ItemType type, ItemBit bit)
    {
        switch (type)
        {
            case ITEM_TYPE_WEAPON:
            {
                switch (bit)
                {
                    case WEAPON_SHIELD:
                    return "Shield:\nReduces knockback from enemies by 80%";
                    case WEAPON_FORK:
                    return "Fork:\nAttacks now shoot forks in the cursor's direction";
                    case WEAPON_SWORD:
                    return "Sword:\nAttacks deal extra damage";
                }
            }break;

            case ITEM_TYPE_ARMOUR:
            {
                return "Armour:\nAdds 1 armor";
            }break;

            case ITEM_TYPE_UTILITY:
            {
                switch (bit)
                {
                    case UTILITY_GRENADE:
                    return "Grenade:\nAttacks now explode, dealing area of effect damage";
                    case UTILITY_POTION:
                    return "Potion:\nAttack speed is increased by 300%";
                    case UTILITY_ROPE:
                    return "Rope:\nNothing";
                }
            }break;


        }
    }

    void renderItems()
    {
        glm::vec2 itemFrameSize = {125,125};
        f32 itemCount = 0;
        glm::vec2 buttonSize = {
            ENGINE.getScreenSize().x / 3.0f,
            ENGINE.getScreenSize().y / 6.5f
        };
        f32 fontSize = 500.0f;
        for (ItemBit i = 0; i != WEAPON_COUNT; i++)
        {
            if (hasWeapon(i))
            {
                if (ENGINE.uiButtonHover(glm::vec2(itemFrameSize.x * itemCount, 0),
                                        itemFrameSize,
                                        AnchorPoint::C,
                                        getTextureIdByItem(ITEM_TYPE_WEAPON, i),
                                        getTextureIdByItem(ITEM_TYPE_WEAPON, i)))
                {
                    ENGINE.uiButtonText(glm::vec2(cursorPosition.x, (itemFrameSize.y + buttonSize.y) / 2.0f),
                                        buttonSize,
                                        AnchorPoint::C,
                                        ID(BUTTON),
                                        fontSize,
                                        getItemDescription(ITEM_TYPE_WEAPON, i));
                }
                itemCount++;
            }
        }

        for (ItemBit i = 0; i != ARMOUR_COUNT; i++)
        {
            if (hasArmour(i))
            {
                if (ENGINE.uiButtonHover(glm::vec2(itemFrameSize.x * itemCount, 0),
                                        itemFrameSize,
                                        AnchorPoint::C,
                                        getTextureIdByItem(ITEM_TYPE_ARMOUR, i),
                                        getTextureIdByItem(ITEM_TYPE_ARMOUR, i)))
                {
                    ENGINE.uiButtonText(glm::vec2(cursorPosition.x, (itemFrameSize.y + buttonSize.y) / 2.0f),
                                        buttonSize,
                                        AnchorPoint::C,
                                        ID(BUTTON),
                                        fontSize,
                                        getItemDescription(ITEM_TYPE_ARMOUR, i));
                }
                itemCount++;
            }
        }

        for (ItemBit i = 0; i != UTILITY_COUNT; i++)
        {
            if (hasUtility(i))
            {
                if (ENGINE.uiButtonHover(glm::vec2(itemFrameSize.x * itemCount, 0),
                                        itemFrameSize,
                                        AnchorPoint::C,
                                        getTextureIdByItem(ITEM_TYPE_UTILITY, i),
                                        getTextureIdByItem(ITEM_TYPE_UTILITY, i)))
                {
                    ENGINE.uiButtonText(glm::vec2(cursorPosition.x, (itemFrameSize.y + buttonSize.y) / 2.0f),
                                        buttonSize,
                                        AnchorPoint::C,
                                        ID(BUTTON),
                                        fontSize,
                                        getItemDescription(ITEM_TYPE_UTILITY, i));
                }
                itemCount++;
            }
        }
    }

    u32 hasWeapon(ItemBit weapon)
    {
        return itemSet.weapons >> weapon & 1;
    }

    u32 hasArmour(ItemBit armour)
    {
        return itemSet.armours >> armour & 1;
    }

    bool hasUtility(ItemBit utility)
    {
        return itemSet.utility & BIT(utility);
    }

    void itemPickupSystem()
    {
        SpriteComponent& playerSprite = entityRegistry.getComponent<SpriteComponent>(playerEId);
        for (Entity eId : entityRegistry.iterateEntities<ItemComponent, SpriteComponent>())
        {
            SpriteComponent& sp = entityRegistry.getComponent<SpriteComponent>(eId);
            if (isCollision(playerSprite, sp))
            {
                ItemComponent& item = entityRegistry.getComponent<ItemComponent>(eId);
                logInfo("Before pickup: %d %d %d", itemSet.weapons, itemSet.armours, itemSet.utility);

                switch (item.itemType)
                {
                    case ITEM_TYPE_WEAPON:
                    {
                        itemSet.weapons |= BIT(item.itemId);
                    }break;

                    case ITEM_TYPE_ARMOUR:
                    {
                        itemSet.armours |= BIT(item.itemId);
                    }break;

                    case ITEM_TYPE_UTILITY:
                    {
                        itemSet.utility|= BIT(item.itemId);
                    }break;
                }

                logInfo("After pickup: %d %d %d", itemSet.weapons, itemSet.armours, itemSet.utility);
                entityRegistry.markEntityForDeletion(eId);
            }
        }

        entityRegistry.removeMarkedEntities();
    }

    void addPlayer(const glm::vec2 position, const TextureTag tag,
                   const f32 movementSpeed, const f32 healthPoints, const f32 attackPoints)
    {
        const Entity eId = entityRegistry.createEntity();
        playerEId = eId;

        camera.z = DEFAULT_CAMERA_ZOOM;
        camera.x = position.x;
        camera.y = position.y;

        playerRadius = calcRadiusOfEntity(TextureTag::PLAYER);

        entityRegistry.addComponent<SpriteComponent>(eId, position, TEXTURE_SIZES[static_cast<u32>(tag)], getTextureIdByTag(tag));
        entityRegistry.addComponent<EntityComponent>(eId, movementSpeed, healthPoints, 20.0f);
        entityRegistry.addComponent<VelocityComponent>(eId, glm::vec2{}, glm::vec2{});
        entityRegistry.addComponent<TagComponent>(eId, TagComponent::PLAYER);
    }

    void addPlayer(glm::vec2 position)
    {
        playerEId = addEntity(position, PLAYER);

        camera.z = DEFAULT_CAMERA_ZOOM;
        camera.x = position.x;
        camera.y = position.y;

        playerRadius = TEXTURE_SIZES[(u32)ENEMIES_STATS[PLAYER].tag].x / 2.0f;
    }

    Entity addEntity(const glm::vec2 position, const TextureTag tag,
                     const f32 movementSpeed, const f32 healthPoints, const f32 attackPoints)
    {
        Entity eId = entityRegistry.createEntity();

        entityRegistry.addComponent<SpriteComponent>(eId, position, TEXTURE_SIZES[static_cast<u32>(tag)], getTextureIdByTag(tag));
        entityRegistry.addComponent<EntityComponent>(eId, movementSpeed, healthPoints, attackPoints);
        entityRegistry.addComponent<TagComponent>(eId, TagComponent::ENEMY);

        Entity healthBarId = entityRegistry.createEntity();

        entityRegistry.addComponent<SolidColorComponent>(healthBarId, SolidColorComponent{{0, 0}, {TEXTURE_SIZES[(u32)tag].x, HEALTH_BAR_HEIGHT}, {1.0f, 0.0f, 0.0f, 1.0f}});
        entityRegistry.addComponent<FollowComponent>(healthBarId, FollowComponent{eId, {0, TEXTURE_SIZES[(u32)tag].y / 2.0f + HEALTH_BAR_HEIGHT_OFFSET}});
        entityRegistry.addComponent<HealthBarComponent>(healthBarId, HealthBarComponent{{TEXTURE_SIZES[static_cast<u32>(tag)].x, 50.0f}, healthPoints});

        return eId;
    }

    Entity addEntity(glm::vec2 position, EntityStats stats)
    {
        Entity eId = entityRegistry.createEntity();

        entityRegistry.addComponent<SpriteComponent>(eId, position, TEXTURE_SIZES[(u32)stats.tag], getTextureIdByTag(stats.tag));
        entityRegistry.addComponent<EntityComponent>(eId, stats.movementSpeed, stats.healthPoints, stats.attackPoints);
        entityRegistry.addComponent<EnemyComponent>(eId, EnemyComponent::WALK, (f32)ENGINE.getGameTime());
        entityRegistry.addComponent<TagComponent>(eId, TagComponent::ENEMY);

        Entity healthBarId = entityRegistry.createEntity();

        entityRegistry.addComponent<SolidColorComponent>(
                                                         healthBarId, SolidColorComponent{{0, 0}, {TEXTURE_SIZES[(u32)stats.tag].x, HEALTH_BAR_HEIGHT}, {1.0f, 0.0f, 0.0f, 1.0f}});
        entityRegistry.addComponent<FollowComponent>(
                                                     healthBarId, FollowComponent{eId, {0, TEXTURE_SIZES[(u32)stats.tag].y / 2.0f + HEALTH_BAR_HEIGHT_OFFSET}});
        entityRegistry.addComponent<HealthBarComponent>(healthBarId, HealthBarComponent{
                                                            {TEXTURE_SIZES[(u32)stats.tag].x, 50.0f}, stats.healthPoints});

        return eId;
    }

    Entity addEntity(glm::vec2 position, EntityType enemyType)
    {
        return addEntity(position, ENEMIES_STATS[ZOMBIE]);
    }

    void updatePlayerPosition()
    {
        SpriteComponent& playerSprite = entityRegistry.getComponent<SpriteComponent>(playerEId);
        EntityComponent& playerEntity = entityRegistry.getComponent<EntityComponent>(playerEId);

        if (actionState.zoomIn == KeyState::HOLD)
        {
            camera.z += camera.z * deltaTime;
        }

        if (actionState.zoomOut == KeyState::HOLD)
        {
            camera.z -= camera.z * deltaTime;
        }

        glm::vec2 direction{0.0f, 0.0f};
        if (actionState.walkUp == KeyState::HOLD || actionState.walkUp == KeyState::PRESS)
            direction += glm::vec2{0.0f, +1.0f};
        if (actionState.walkDown == KeyState::HOLD || actionState.walkDown == KeyState::PRESS)
            direction += glm::vec2{0.0f, -1.0f};
        if (actionState.walkLeft == KeyState::HOLD || actionState.walkLeft == KeyState::PRESS)
            direction += glm::vec2{-1.0f, 0.0f};
        if (actionState.walkRight == KeyState::HOLD || actionState.walkRight == KeyState::PRESS)
            direction += glm::vec2{+1.0f, 0.0f};


        VelocityComponent& v = entityRegistry.getComponent<VelocityComponent>(playerEId);
        v.targetVel = direction * playerEntity.movementSpeed;

        playerSprite.position = resolveBasePositionCollision(playerSprite.position - v.vel * deltaTime, playerSprite.position, TextureTag::PLAYER);
        f32 len = glm::length(cursorPosition);
        len = fmax(len, 50);
        glm::vec2 offset = glm::normalize(cursorPosition) * len;

        camera.x = playerSprite.position.x + offset.x / 20.0f;
        camera.y = playerSprite.position.y + offset.y / 20.0f;

    }

    void renderCursor()
    {
        ENGINE.uiTexture(cursorPosition, {50.0f,50.0f}, AnchorPoint::C, getTextureIdByTag(TextureTag::CIRCLE));
    }

    void updatePlayerHealth()
    {
        const SpriteComponent& playerSprite = entityRegistry.getComponent<SpriteComponent>(playerEId);
        EntityComponent& playerEntity = entityRegistry.getComponent<EntityComponent>(playerEId);
        VelocityComponent& playerVel = entityRegistry.getComponent<VelocityComponent>(playerEId);

        for (const auto& enemyId: entityRegistry.iterateEntities<TagComponent, SpriteComponent, EntityComponent>())
        {
            if (entityRegistry.getComponent<TagComponent>(enemyId) == TagComponent::PLAYER)
                continue;
            const SpriteComponent& enemySprite = entityRegistry.getComponent<SpriteComponent>(enemyId);
            const EntityComponent& enemyEntity = entityRegistry.getComponent<EntityComponent>(enemyId);

            if (isCollision(playerSprite, enemySprite))
            {
                f32 armourCount = (f32)(hasArmour(ARMOUR_CHESTPLATE) + hasArmour(ARMOUR_HELMET) + hasArmour(ARMOUR_PANTS));
                playerEntity.healthPoints -= enemyEntity.attackPoints / (1.0f + armourCount);

                glm::vec2 dir = glm::normalize(playerSprite.position - enemySprite.position);
                if (hasWeapon(WEAPON_SHIELD))
                    playerVel.vel += dir * 100.0f;
                else
                    playerVel.vel += dir * 500.0f;

                if (playerEntity.healthPoints <= 0.0f)
                {
                    gameState = GAME_LOST;
                    return;
                }
            }
        }
    }

    void updatePlayerAttack()
    {
        if (playerAttackTimer > 0.0f)
        {
            playerAttackTimer -= deltaTime;
            return;
        }
        if (actionState.attack != KeyState::PRESS && actionState.attack != KeyState::HOLD)
        {
            return;
        }


        if (hasWeapon(WEAPON_FORK))
        {
            playerAttackTimer = 0.4f - (0.3f * (f32)hasUtility(UTILITY_POTION));
            // cursorPosition is not affected by the camera position in calculations.
            //To convert cursorPosition to actual world Position you have to add the camera position to it
            EntityComponent& playerEntity = entityRegistry.getComponent<EntityComponent>(playerEId);
            TagComponent tag = entityRegistry.getComponent<TagComponent>(playerEId);
            const Entity projectileId = entityRegistry.createEntity();
            const glm::vec2 directionVector = glm::normalize(cursorPosition + glm::vec2{camera.x, camera.y} - entityRegistry.getComponent<SpriteComponent>(playerEId).position);
            entityRegistry.addComponent<ProjectileComponent>(projectileId, directionVector, 400.0f, playerEntity.attackPoints + (50.0f * (f32)hasWeapon(WEAPON_SWORD)), false);
            entityRegistry.addComponent<SpriteComponent>(projectileId,
                                                         entityRegistry.getComponent<SpriteComponent>(playerEId).position,
                                                         TEXTURE_SIZES[static_cast<u32>(TextureTag::PROJECTILE)],
                                                         getTextureIdByTag(TextureTag::PROJECTILE),
                                                         (f32)(atan2(directionVector.y, directionVector.x) - PI / 2.0f));
        }
    }

    void updatePlayer()
    {
        updatePlayerPosition();
        updatePlayerAttack();
        updatePlayerHealth();
    }

    /* Custom collision */
    void updateEnemies()
    {
        f32 time = ENGINE.getGameTime();
        for (EnemyComponent& enemy : entityRegistry.iterateComponents<EnemyComponent>())
        {
            if (enemy.phase == EnemyComponent::WALK && time - enemy.startTime > 5.0f)
            {
                enemy.phase = EnemyComponent::SHOOT;
                enemy.startTime = time;
            }else if (enemy.phase == EnemyComponent::WAIT && time - enemy.startTime > 1.0f)
            {
                enemy.phase = EnemyComponent::WALK;
                enemy.startTime = time;
            }
        }

        const SpriteComponent& playerSprite = entityRegistry.getComponent<SpriteComponent>(playerEId);

        for (const auto& enemyEntityId: entityRegistry.iterateEntities<SpriteComponent, EntityComponent, TagComponent>())
        {
            SpriteComponent& enemySprite = entityRegistry.getComponent<SpriteComponent>(enemyEntityId);
            const glm::vec2 enemyVector{
                enemySprite.position.x - playerSprite.position.x,
                enemySprite.position.y - playerSprite.position.y
            };

            if (glm::length(enemyVector) > ENEMY_DETECTION_RADIUS)
            {
                continue;
            }

            if (entityRegistry.getComponent<TagComponent>(enemyEntityId) == TagComponent::PLAYER)
            {
                continue;
            }

            const glm::vec2 normalizedEnemyVector = glm::normalize(enemyVector);
            EntityComponent& enemyEntity = entityRegistry.getComponent<EntityComponent>(enemyEntityId);
            EnemyComponent& enemy = entityRegistry.getComponent<EnemyComponent>(enemyEntityId);
            if (enemy.phase == EnemyComponent::WALK)
            {
                const f32 distance = deltaTime * enemyEntity.movementSpeed;

                const glm::vec2 nextPosition{
                    enemySprite.position.x - normalizedEnemyVector.x * distance,
                    enemySprite.position.y - normalizedEnemyVector.y * distance
                };
                glm::vec2 newPosition = resolveBasePositionCollision(enemySprite.position, nextPosition, TextureTag::ENEMY);

                // collision with player
                const glm::vec2 vectorBetween{
                    playerSprite.position.x - newPosition.x,
                    playerSprite.position.y - newPosition.y
                };

                const f32 distanceBetween = glm::length(vectorBetween);

                enemySprite.position.x = newPosition.x;
                enemySprite.position.y = newPosition.y;
            } else if (enemy.phase == EnemyComponent::SHOOT)
            {
                enemy.phase = EnemyComponent::WAIT;
                Entity proj = entityRegistry.createEntity();
                glm::vec2 dir = glm::normalize(playerSprite.position - enemySprite.position);
                entityRegistry.addComponent<ProjectileComponent>(proj,
                                                                 dir,
                                                                 250.0f,
                                                                 enemyEntity.attackPoints,
                                                                 true);
                entityRegistry.addComponent<SpriteComponent>(proj,
                                                             enemySprite.position,
                                                             TEXTURE_SIZES[static_cast<u32>(TextureTag::PROJECTILE)],
                                                             getTextureIdByTag(TextureTag::PROJECTILE),
                                                             (f32)(atan2(dir.y, dir.x) - PI / 2.0f));
            }
        }
    }

    void renderSprites() const
    {
        u32 cnt = 0;
        const ComponentVector<SpriteComponent>& sprites = entityRegistry.getComponentVector<SpriteComponent>();
        Entity* const sortedEntityIdsArray = static_cast<Entity*>(ENGINE.temporaryAlloc(sprites.size() * sizeof(Entity)));

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
                if (cursorPosition.x < 0)
                {
                    sprite.size.x = -abs(sprite.size.x);
                }
                else
                {
                    sprite.size.x = abs(sprite.size.x);
                }
            }
            else if (sprite.textureId == getTextureIdByTag(TextureTag::ENEMY))
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
            ENGINE.drawTexturedQuad(sprite.position, sprite.size, sprite.textureId, sprite.rotation);
        }
        ENGINE.addLight(sprites.getComponent(playerEId).position, 100.0f, {1.0f, 1.0f, 1.0f, 1.0f});

        // Uncomment this if you want the player to be rendered over every other sprite
        //const SpriteComponent playerSprite = sprites.getComponent(PLAYER_ID);
        //draw({ playerSprite.x, playerSprite.y }, getQuadSize() *= 2, playerSprite.textureId);

        for (const SolidColorComponent& solidColor: entityRegistry.iterateComponents<SolidColorComponent>())
        {
            ENGINE.drawColoredQuad(solidColor.position, solidColor.size, solidColor.color, solidColor.rotation);
        }
    }

    void velocitySystem()
    {
        for (Entity vEid: entityRegistry.iterateEntities<VelocityComponent>())
        {
            VelocityComponent& v = entityRegistry.getComponent<VelocityComponent>(vEid);
            SpriteComponent& sp = entityRegistry.getComponent<SpriteComponent>(vEid);
            v.vel += (v.targetVel - v.vel) * 20.0f * deltaTime;
            sp.position += v.vel * deltaTime;
        }
    }

    void startGame()
    {
        init(MAP_SIZE_X, MAP_SIZE_Y);
        addPlayer(startPosition, TextureTag::PLAYER, DEFAULT_PLAYER_SPEED, 200, 50);

        animations = (animation_id*)ENGINE.globalAlloc(sizeof(animation_id) * ANIMATION_STATE_COUNT);
        // for (u32 i = 0; i < ANIMATION_STATE_COUNT; ++i)
        // {
        //     GAME->animations[i] = ANIMATION->createAnimation(textureIds, ARRAY_COUNT(textureIds), 0.2f);
        // }

        updateFollowers();

        SpriteComponent& playerSp = entityRegistry.getComponent<SpriteComponent>(playerEId);

        Entity itemEId = entityRegistry.createEntity();

        entityRegistry.addComponent<ItemComponent>(itemEId, ITEM_TYPE_WEAPON, WEAPON_FORK);
        entityRegistry.addComponent<SpriteComponent>(itemEId, playerSp.position + glm::vec2(150, 0), glm::vec2(25, 25), getTextureIdByTag(TextureTag::FORK));

        itemEId = entityRegistry.createEntity();

        entityRegistry.addComponent<ItemComponent>(itemEId, ITEM_TYPE_WEAPON, WEAPON_SHIELD);
        entityRegistry.addComponent<SpriteComponent>(itemEId, playerSp.position + glm::vec2(-150, 0), glm::vec2(25, 25), getTextureIdByTag(TextureTag::SHIELD));

        itemEId = entityRegistry.createEntity();

        entityRegistry.addComponent<ItemComponent>(itemEId, ITEM_TYPE_WEAPON, WEAPON_SWORD);
        entityRegistry.addComponent<SpriteComponent>(itemEId, playerSp.position + glm::vec2(0, 150), glm::vec2(25, 25), getTextureIdByTag(TextureTag::SWORD));

        ENGINE.printGlobalAllocations(false);
        gameState = GAME_RUNNING;
    }

    void moveProjectiles()
    {
        for (const auto& projectileId : entityRegistry.iterateEntities<SpriteComponent, ProjectileComponent>())
        {
            SpriteComponent& projectileSprite = entityRegistry.getComponent<SpriteComponent>(projectileId);
            ProjectileComponent& projectile = entityRegistry.getComponent<ProjectileComponent>(projectileId);
            projectileSprite.position += projectile.direction * projectile.speed * deltaTime;

            if (!isOnScreen(projectileSprite.position))
            {
                entityRegistry.markEntityForDeletion(projectileId);
                continue;
            }

            for (const auto& enemyId: entityRegistry.iterateEntities<SpriteComponent, EntityComponent, TagComponent>())
            {
                if (entityRegistry.getComponent<TagComponent>(enemyId) == TagComponent::ENEMY && projectile.isEnemy ||
                    entityRegistry.getComponent<TagComponent>(enemyId) == TagComponent::PLAYER && !projectile.isEnemy)
                    continue;

                const SpriteComponent& enemySprite = entityRegistry.getComponent<SpriteComponent>(enemyId);
                const f32 distanceBetween = glm::distance(enemySprite.position, projectileSprite.position);
                if (distanceBetween <= std::min(enemySprite.size.x, enemySprite.size.y) / 2)
                    //if (isCollision(enemySprite, projectileSprite))
                {
                    ProjectileComponent& proj = entityRegistry.getComponent<ProjectileComponent>(projectileId);
                    // subtract shooter's attack from enemy's health
                    entityRegistry.getComponent<EntityComponent>(enemyId).healthPoints -=
                        projectile.damage;

                    if (entityRegistry.getComponent<EntityComponent>(enemyId).healthPoints <= 0.0f)
                    {
                        entityRegistry.markEntityForDeletion(enemyId);
                        if (enemyId == playerEId)
                        {
                            gameState = GAME_LOST;
                            return;
                        }
                    }

                    //Emit Particles
                    ENGINE.emitParticles(20,
                                         enemySprite.position,
                                         50.0f,
                                         0.0f,
                                         proj.direction,
                                         glm::vec4(0.7f, 0, 0, 1.0f),
                                         glm::vec4(0.4f, 0, 0, 0.0f),
                                         glm::vec2(4.0f),
                                         3.0f,
                                         0.5f);
                    // delete projectile
                    entityRegistry.markEntityForDeletion(projectileId);
                }
            }
        }
        entityRegistry.removeMarkedEntities();
    }

    glm::vec2 getCameraUnits() const
    {
        return ENGINE.getScreenSize() / camera.z;
    }

    glm::vec2 getCameraLeftTopCorner() const
    {
        return {camera.x - getCameraUnits().x / 2.0f, camera.y + getCameraUnits().y / 2.0f};
    }

    glm::vec2 getCameraRightTopCorner() const
    {
        return {camera.x + getCameraUnits().x / 2.0f, camera.y + getCameraUnits().y / 2.0f};
    }

    glm::vec2 getCameraLeftBottomCorner() const
    {
        return {camera.x - getCameraUnits().x / 2.0f, camera.y - getCameraUnits().y / 2.0f};
    }

    glm::vec2 getCameraRightBottomCorner() const
    {
        return {camera.x + getCameraUnits().x / 2.0f, camera.y - getCameraUnits().y / 2.0f};
    }

    bool isOnScreen(glm::vec2 pos) const {
        glm::vec2 leftBottomCorner = getCameraLeftBottomCorner();
        glm::vec2 rightTopCorner = getCameraRightTopCorner();
        return pos.x >= leftBottomCorner.x && pos.x <= rightTopCorner.x &&
            pos.y >= leftBottomCorner.y && pos.y <= rightTopCorner.y;
    }

    void playerToggleVroom()
    {
        EntityComponent& player = entityRegistry.getComponent<EntityComponent>(playerEId);
        isVroomOn = !isVroomOn;
        if (isVroomOn)
            player.movementSpeed = 10.0f * DEFAULT_PLAYER_SPEED;
        else
            player.movementSpeed = DEFAULT_PLAYER_SPEED;
    }

    glm::uvec2 getTileByPosition(glm::vec2 position) const
    {
        // we substract map.quadSize / 2 because we draw map.tiles from the center
        return (glm::uvec2)(position / map.quadSize);
    }

    glm::vec2 getPlayerSpritePosition() const
    {
        return entityRegistry.getComponent<SpriteComponent>(playerEId).position;
    }

    // position used for collisions
    glm::vec2 getPlayerBasePosition() const
    {
        assert(TEXTURE_SIZES[TAG(PLAYER)].x <= TEXTURE_SIZES[TAG(PLAYER)].y && "Ratia texturii (x/y) playerului este prea mare");
        glm::vec2 spritePos = getPlayerSpritePosition();
        return {spritePos.x, spritePos.y - (TEXTURE_SIZES[TAG(PLAYER)].y - TEXTURE_SIZES[TAG(PLAYER)].x) / 2};
    }

    // position used for collisions
    glm::vec2 getBasePosition(glm::vec2 spritePos, TextureTag tag) const
    {
        assert(TEXTURE_SIZES[(u32)tag].x <= TEXTURE_SIZES[(u32)tag].y && "Ratia texturii (x/y) este prea mare");
        return {spritePos.x, spritePos.y - (TEXTURE_SIZES[(u32)tag].y - TEXTURE_SIZES[(u32)tag].x) / 2};
    }

    glm::vec2 playerBaseToSpritePosition(glm::vec2 basePosition) const
    {
        return {basePosition.x, basePosition.y + (TEXTURE_SIZES[TAG(PLAYER)].y - TEXTURE_SIZES[TAG(PLAYER)].x) / 2};
    }

    glm::vec2 baseToSpritePosition(glm::vec2 basePosition, TextureTag tag) const
    {
        return {basePosition.x, basePosition.y + (TEXTURE_SIZES[(u32)tag].y - TEXTURE_SIZES[(u32)tag].x) / 2};
    }

    f32 calcRadiusOfEntity(TextureTag tag)
    {
        return TEXTURE_SIZES[(u32)tag].x / 2.0f;
    }

    void resolveCollision(glm::vec2 oldPos, glm::vec2& pos, u32 radius)
    {
        glm::uvec2 tile = getTileByPosition(pos);
        assert(tile.x > 0 && tile.y > 0);
        glm::vec2 normDirection = pos - oldPos;
        for (u32 i = tile.y - 1; i <= tile.y + 1; ++i)
        {
            for (u32 j = tile.x - 1; j <= tile.x + 1; ++j)
            {
                u32 tileIndex = i * map.size.x + j;
                if (map.tiles[tileIndex] >= getTextureIdByTag(TextureTag::COLLISION_START) &&
                    map.tiles[tileIndex] <= getTextureIdByTag(TextureTag::COLLISION_END))
                {
                    glm::uvec2 tile{j,i};
                    glm::vec2 leftBottomCorner = getLeftBottomCornerByTile(tile);
                    glm::vec2 nearestPoint{
                        std::max(leftBottomCorner.x, std::min(pos.x, leftBottomCorner.x + map.quadSize.x)),
                        std::max(leftBottomCorner.y, std::min(pos.y, leftBottomCorner.y + map.quadSize.y))
                    };
                    glm::vec2 rayToNearest = nearestPoint - pos;
                    //assert((rayToNearest.x || rayToNearest.y) && "rayToNearest nu poate fi 0 deaorece va crapa la normalizare");
                    f32 overlap = radius - glm::length(rayToNearest);
                    if (std::isnan(overlap))
                        overlap = 0;
                    //assert(!std::isnan(overlap) && "Overlap e NaN, incearca sa egalezi cu 0");
                    if (overlap > 0)
                    {
                        if (glm::length(rayToNearest) != 0.0f)
                            pos -= glm::normalize(rayToNearest) * overlap;
                        else
                            pos -= glm::normalize(normDirection) * overlap;
                    }
                }
            }
        }
    }

    glm::vec2 Game::resolveBasePositionCollision(glm::vec2 oldPosition, glm::vec2 position, TextureTag tag)
    {
        glm::vec2 basePos = getBasePosition(position, tag);
        resolveCollision(getBasePosition(oldPosition, tag), basePos, calcRadiusOfEntity(tag));
        return baseToSpritePosition(basePos, tag);
    }

    bool checkFillTile(bool* mat, u32 tileIndex) const
    {
        // return !mat[tileIndex] && map.tiles[tileIndex] >= ID(WALL) && map.tiles[tileIndex] <= ID(WALL_CORNER_TOP_RIGHT);
        return map.tiles[tileIndex] >= ID(WALL) && map.tiles[tileIndex] <= ID(WALL_CORNER_TOP_RIGHT);
    }

    bool fillUtilX(bool* mat, i32 tileX, i32 tileY)
    {
        assert(tileX >= 0 && tileX < map.size.x &&
               tileY >= 0 && tileY < map.size.y);
        bool ok = false;
        i32 j = tileX - 1;
        while (j >= 0 && checkFillTile(mat, tileY * map.size.x + j))
        {
            mat[tileY * map.size.x + j] = true;
            --j;
            ok = true;
        }
        ++tileX;
        while (tileX < map.size.x && checkFillTile(mat, tileY * map.size.x + tileX))
        {
            mat[tileY * map.size.x + tileX] = true;
            ++tileX;
            ok = true;
        }
        if (ok)
        {
            ++j;
            map.walls[map.numberOfWalls++] = {
                getLeftBottomCornerByTile({j, tileY}),
                getLeftBottomCornerByTile({tileX, tileY + 1})
            };
        }
        return ok;
    }

    bool fillUtilY(bool* mat, i32 tileX, i32 tileY)
    {
        assert(tileX >= 0 && tileX < map.size.x &&
               tileY >= 0 && tileY < map.size.y);
        bool ok = false;
        i32 i = tileY - 1;
        while (i >= 0 && checkFillTile(mat, i * map.size.x + tileX))
        {
            mat[i * map.size.x + tileX] = true;
            --i;
            ok = true;
        }
        ++tileY;
        while (tileY < map.size.y && checkFillTile(mat, tileY * map.size.x + tileX))
        {
            mat[tileY * map.size.x + tileX] = true;
            ++tileY;
            ok = true;
        }
        if (ok)
        {
            ++i;
            map.walls[map.numberOfWalls++] = {
                getLeftBottomCornerByTile({tileX, i}),
                getLeftBottomCornerByTile({tileX + 1, tileY})
            };
        }
        return ok;
    }

    void fill()
    {
        bool* mat = (bool*)ENGINE.globalAlloc(sizeof(bool) * map.size.x * map.size.y);
        memset(mat, false, sizeof(bool) * map.size.x * map.size.y);
        for (i32 i = 0; i < map.size.y; ++i)
        {
            for (i32 j = 0; j < map.size.x; ++j)
            {
                i32 tileIndex = i * map.size.x + j;
                if (!mat[tileIndex] && checkFillTile(mat, tileIndex)) {
                    mat[tileIndex] = true;
                    if (!fillUtilX(mat, j, i) && !fillUtilY(mat, j, i))
                    {
                        map.walls[map.numberOfWalls++] = {
                            getLeftBottomCornerByTile({j, i}),
                            getLeftBottomCornerByTile({j + 1, i + 1})
                        };
                    }
                }
            }
        }
        ENGINE.globalFree(mat);
    }

    void init(const u32 sizeX, const u32 sizeY)
    {
        map.size.x = sizeX;
        map.size.y = sizeY;
        map.tiles = (texture_id*)ENGINE.globalAlloc(sizeof(texture_id) * sizeY * sizeX);
        map.walls = (Map::Wall*)ENGINE.globalAlloc(sizeof(Map::Wall) * MAX_WALLS);
        map.numberOfWalls = 0;
        for (u32 i = 0; i < sizeY * sizeX; ++i)
        {
            map.tiles[i] = ID(NONE);
        }
        map.quadSize = {QUAD_SIZE, QUAD_SIZE};
        generateMapLevel();
        fill();
    }

    glm::vec2 getCenterPositionByTile(glm::uvec2 tile) const
    {
        return glm::vec2((f32)tile.x, (f32)tile.y) * map.quadSize + map.quadSize / 2.0f;
    }

    glm::vec2 getLeftBottomCornerByTile(glm::uvec2 tile) const
    {
        return glm::vec2((f32)tile.x, (f32)tile.y) * map.quadSize;
    }

    void render() const
    {
        for (u32 i = 0; i < map.tilesArrSize; ++i)
        {
            glm::uvec2* t = &map.tilesArr[i];
            glm::uvec2 tile = map.tilesArr[i];
            glm::vec2 position = getCenterPositionByTile(tile);
            u32 tileIndex = tile.y * map.size.x + tile.x;
            ENGINE.drawTexturedQuad(position, map.quadSize, map.tiles[tileIndex], 0);
        }
        // logDebug("Nr: %d", map.numberOfWalls);
        for (u32 i = 0; i < map.numberOfWalls; ++i)
        {
            glm::vec2 center = (map.walls[i].leftBottom + map.walls[i].topRight) / 2.0f;
            glm::vec2 size = map.walls[i].topRight - map.walls[i].leftBottom;

            // logDebug("C: (%f, %f), S: (%f, %f)", center.x, center.y, size.x, size.y);
            ENGINE.addLightBlocker(center, size);
            // ENGINE.drawTexturedQuad(center, size, ID(DEBUG_OVERLAY), 0);
        }
    }

    glm::vec2 getQuadSize() const
    {
        return map.quadSize;
    }

    void generateMapLevel()
    {
        assert(map.size.x >= ROOM.MAX.x && map.size.y >= ROOM.MAX.y);
        u64 seed = std::random_device()();

        if (ROOM.MIN.x % 2)
        {
            ROOM.MIN.x += 1;
        }

        if (ROOM.MAX.x % 2)
        {
            ROOM.MAX.x += 1;
        }

        if (ROOM.MIN.y % 2)
        {
            ROOM.MIN.y -= 1;
        }

        if (ROOM.MAX.y % 2)
        {
            ROOM.MAX.y -= 1;
        }

        assert(ROOM.MIN.x <= ROOM.MAX.x && ROOM.MIN.y <= ROOM.MAX.y);

        // transform to constexpr
        const glm::uvec2 ROOM_AVERAGE{
            (ROOM.MIN.x + ROOM.MAX.x) / 2,
            (ROOM.MIN.y + ROOM.MAX.y) / 2
        };

        u32 leftVerticalAxis = 1;
        u32 rightVerticalAxis = map.size.x - 1;

        for (u32 prevYPos = map.size.y / 2 - ROOM_AVERAGE.y, prevWidth, prevHeight, prevLeftVerticalAxis, roomRight, rooms = 0; ; ++rooms)
        {
            u32 height = (u32)ENGINE.randomRangeU64(seed, ROOM.MIN.y, ROOM.MAX.y) & (~1u);
            u32 width = (u32)ENGINE.randomRangeU64(seed, ROOM.MIN.x, ROOM.MAX.x) & (~1u);

            if (leftVerticalAxis + width > rightVerticalAxis)
            {
                // // right
                // map.walls[map.numberOfWalls++] = {
                //     getLeftBottomCornerByTile({prevLeftVerticalAxis + prevWidth - 1, prevYPos}),
                //     getLeftBottomCornerByTile({prevLeftVerticalAxis + prevWidth, prevYPos + prevHeight})
                // };
                break;
            }

            const u32 yPos = (u32)ENGINE.randomRangeU64(seed, std::max(0, (i32)prevYPos - (i32)ROOM.MIN.y), std::min(map.size.y - height, prevYPos + ROOM.MIN.y));
            for (u32 i = 1; i + 1 < height; ++i)
            {
                for (u32 j = 1; j + 1 < width; ++j)
                {
                    map.tiles[leftVerticalAxis + (yPos + i) * map.size.x + j] = ID(FLOOR);
                }
            }

            // WALLS
            map.tiles[leftVerticalAxis + yPos * map.size.x] =  ID(WALL_CORNER_BOTTOM_LEFT);
            map.tiles[leftVerticalAxis + yPos * map.size.x + width - 1] = ID(WALL_CORNER_BOTTOM_RIGHT);
            map.tiles[leftVerticalAxis + (yPos + height - 1) * map.size.x] = ID(WALL_CORNER_TOP_LEFT);
            map.tiles[leftVerticalAxis + (yPos + height - 1) * map.size.x + width - 1] = ID(WALL_CORNER_TOP_RIGHT);

            // // bottom
            // map.walls[map.numberOfWalls++] = {
            //     getLeftBottomCornerByTile({leftVerticalAxis, yPos}),
            //     getLeftBottomCornerByTile({leftVerticalAxis + width, yPos + 1})
            // };
            // // top
            // map.walls[map.numberOfWalls++] = {
            //     getLeftBottomCornerByTile({leftVerticalAxis, yPos + height - 1}),
            //     getLeftBottomCornerByTile({leftVerticalAxis + width, yPos + height})
            // };

            // if (rooms == 0)
            // {
            //     // left
            //     map.walls[map.numberOfWalls++] = {
            //         getLeftBottomCornerByTile({leftVerticalAxis, yPos}),
            //         getLeftBottomCornerByTile({leftVerticalAxis + 1, yPos + height})
            //     };
            // }


            for (u32 i = 1; i + 1 < height; ++i)
            {
                map.tiles[leftVerticalAxis + (yPos + i) * map.size.x] = ID(WALL_LEFT);
                map.tiles[leftVerticalAxis + (yPos + i) * map.size.x + width - 1] = ID(WALL_RIGHT);
            }

            for (u32 j = 1; j + 1 < width; ++j)
            {
                map.tiles[leftVerticalAxis + yPos * map.size.x + j] = ID(WALL_BOTTOM);
                map.tiles[leftVerticalAxis + (yPos + height - 1) * map.size.x + j] = ID(WALL_TOP);
            }

            // logDebug("Tile: (%u, %u)", leftVerticalAxis + 2, yPos + 2);
            // logDebug("Tile: (%u, %u)", leftVerticalAxis + width - 3, yPos + 2);
            // logDebug("Tile: (%u, %u)", leftVerticalAxis + 2, yPos + height - 3);
            // logDebug("Tile: (%u, %u)", leftVerticalAxis + width - 3, yPos + height - 3);
            // logDebug("Pos: %f", baseToSpritePosition(
            //         getCenterPositionByTile(glm::uvec2{leftVerticalAxis + 2, yPos + 2}),
            //         ENEMIES_STATS[ZOMBIE].tag));
            // logDebug("");
            addEntity(
                      baseToSpritePosition(
                                           getCenterPositionByTile(glm::uvec2{leftVerticalAxis + 3, yPos + 3}),
                                           ENEMIES_STATS[ZOMBIE].tag),
                      ZOMBIE);
            addEntity(
                      baseToSpritePosition(
                                           getCenterPositionByTile(glm::uvec2{leftVerticalAxis + width - 4, yPos + 3}),
                                           ENEMIES_STATS[ZOMBIE].tag),
                      ZOMBIE);
            addEntity(
                      baseToSpritePosition(
                                           getCenterPositionByTile(glm::uvec2{leftVerticalAxis + 3, yPos + height - 4}),
                                           ENEMIES_STATS[ZOMBIE].tag),
                      ZOMBIE);
            addEntity(
                      baseToSpritePosition(
                                           getCenterPositionByTile(glm::uvec2{leftVerticalAxis + width - 4, yPos + height - 4}),
                                           ENEMIES_STATS[ZOMBIE].tag),
                      ZOMBIE);

            const u32 prevRoomRight = roomRight;
            roomRight = leftVerticalAxis + width - 1;
            prevLeftVerticalAxis = leftVerticalAxis;
            leftVerticalAxis += width + (u32)ENGINE.randomRangeU64(seed, ROOM.MIN.x / 2, std::max(ROOM.MIN.x, ROOM.MAX.x / 2)) & (~1u);

            if (rooms != 0)
            {
                // MIDDLE TUNNEL
                u32 startY = prevYPos + prevHeight / 2;
                u32 stopY = yPos + height / 2;
                if (startY >= stopY)
                    std::swap(startY, stopY);

                startY -= 2;
                stopY += 1;

                u32 baseX = (prevRoomRight + prevLeftVerticalAxis) / 2 + 1;

                map.tiles[startY * map.size.x + baseX - 2] = ID(WALL_CORNER_BOTTOM_LEFT);
                map.tiles[startY * map.size.x + baseX - 1] = ID(WALL_BOTTOM);
                map.tiles[startY * map.size.x + baseX]     = ID(WALL_BOTTOM);
                map.tiles[startY * map.size.x + baseX + 1] = ID(WALL_CORNER_BOTTOM_RIGHT);
                for (u32 i = startY + 1; i < stopY; ++i)
                {
                    map.tiles[i * map.size.x + baseX - 2] = ID(WALL_LEFT);
                    map.tiles[i * map.size.x + baseX - 1] = ID(FLOOR);
                    map.tiles[i * map.size.x + baseX]     = ID(FLOOR);
                    map.tiles[i * map.size.x + baseX + 1] = ID(WALL_RIGHT);
                }
                map.tiles[stopY * map.size.x + baseX - 2] = ID(WALL_CORNER_TOP_LEFT);
                map.tiles[stopY * map.size.x + baseX - 1] = ID(WALL_TOP);
                map.tiles[stopY * map.size.x + baseX]     = ID(WALL_TOP);
                map.tiles[stopY * map.size.x + baseX + 1] = ID(WALL_CORNER_TOP_RIGHT);

                // LEFT TUNNEL
                u32 startX = prevRoomRight;
                u32 stopX = (prevRoomRight + prevLeftVerticalAxis) / 2 - 1;
                u32 baseY = prevYPos + prevHeight / 2;
                // entrance - left room
                map.tiles[(baseY - 2) * map.size.x + startX] = ID(WALL_CORNER_BOTTOM_RIGHT);
                //map.walls[map.numberOfWalls++] = getLeftBottomCornerByTile({startX, baseY - 2});
                map.tiles[(baseY - 1) * map.size.x + startX] = ID(FLOOR);
                map.tiles[ baseY      * map.size.x + startX] = ID(FLOOR);
                map.tiles[(baseY + 1) * map.size.x + startX] = ID(WALL_CORNER_TOP_RIGHT);
                // left tunnel
                for (u32 i = startX + 1; i < stopX; ++i)
                {
                    map.tiles[(baseY - 2) * map.size.x + i] = ID(WALL_BOTTOM);
                    map.tiles[(baseY - 1) * map.size.x + i] = ID(FLOOR);
                    map.tiles[ baseY      * map.size.x + i] = ID(FLOOR);
                    map.tiles[(baseY + 1) * map.size.x + i] = ID(WALL_TOP);
                }
                // end of left tunnel
                map.tiles[(baseY - 1) * map.size.x + stopX] = ID(FLOOR);
                map.tiles[ baseY      * map.size.x + stopX] = ID(FLOOR);

                // RIGHT TUNNEL
                baseY = yPos + height / 2;
                startX = (prevRoomRight + prevLeftVerticalAxis) / 2 + 3;
                stopX = prevLeftVerticalAxis;
                // entrance - right room
                map.tiles[(baseY - 2) * map.size.x + stopX] = ID(WALL_CORNER_BOTTOM_RIGHT);
                map.tiles[(baseY - 1) * map.size.x + stopX] = ID(FLOOR);
                map.tiles[ baseY      * map.size.x + stopX] = ID(FLOOR);
                map.tiles[(baseY + 1) * map.size.x + stopX] = ID(WALL_CORNER_TOP_RIGHT);
                // right tunnel
                for (u32 i = startX; i < stopX; ++i)
                {
                    map.tiles[(baseY - 2) * map.size.x + i] = ID(WALL_BOTTOM);
                    map.tiles[(baseY - 1) * map.size.x + i] = ID(FLOOR);
                    map.tiles[ baseY      * map.size.x + i] = ID(FLOOR);
                    map.tiles[(baseY + 1) * map.size.x + i] = ID(WALL_TOP);
                }
                // end of right tunnel
                map.tiles[(baseY - 1) * map.size.x + startX - 1] = ID(FLOOR);
                map.tiles[ baseY      * map.size.x + startX - 1] = ID(FLOOR);
            }
            else
            {
                glm::vec2 cornerLeftBottom = glm::vec2((f32)prevLeftVerticalAxis, (f32)yPos);
                cornerLeftBottom *= map.quadSize;
                startPosition = playerBaseToSpritePosition(cornerLeftBottom + map.quadSize * glm::vec2{(f32)width / 2.0f, (f32)height / 2.0f});
            }

            prevWidth = width;
            prevHeight = height;
            prevYPos = yPos;
        }

        map.tilesArrSize = 0;
        for (u32 i = 0; i < map.size.y; ++i)
        {
            for (u32 j = 0; j < map.size.x; ++j)
            {
                if (map.tiles[i * map.size.x + j] != ID(NONE))
                    ++map.tilesArrSize;
            }
        }
        map.tilesArr = (glm::uvec2*)ENGINE.globalAlloc(map.tilesArrSize * sizeof(glm::uvec2));
        glm::uvec2* debug = map.tilesArr;
        logInfo("%d", map.tilesArrSize);
        u32 cnt = 0;
        for (u32 i = 0; i < map.size.y; ++i)
        {
            for (u32 j = 0; j < map.size.x; ++j)
            {
                if (map.tiles[i * map.size.x + j] != ID(NONE))
                    map.tilesArr[cnt++] = glm::uvec2{j,i};
            }
        }
    }

    bool menuButton(glm::vec2 pos, const char* text)
    {
        buttonSize = {
            ENGINE.getScreenSize().x / 3.0f,
            ENGINE.getScreenSize().y / 6.5f
        };
        return ENGINE.uiButtonHoverText(pos,
                                        buttonSize,
                                        AnchorPoint::C,
                                        ID(BUTTON),
                                        ID(BUTTON_PRESSED),
                                        1000.0f,
                                        text) &&
            actionState.menuInteract == KeyState::PRESS;
    }
};

extern "C"
__declspec(dllexport)
void gameInit() {
    GAME->gameInit();
}

extern "C"
__declspec(dllexport)
void gameInput() {
    GAME->gameInput();
}

extern "C"
__declspec(dllexport)
void gameUpdate(f64& deltaTime) {
    GAME->deltaTime = (f32)deltaTime;
    GAME->gameUpdate();
    deltaTime = (f64)GAME->deltaTime;
}

extern "C"
__declspec(dllexport)
void gameRender(f64 deltaTime) {
    GAME->gameRender();
}
