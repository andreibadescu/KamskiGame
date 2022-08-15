#include "headers/Defines.h"
#include "headers/Components.h"
#include <random>

struct GameState {
    // actually wtf was I smoking
    u32 itemId;
    
    enum CombatPhase
    {
        COMBAT_PHASE_OFF,
        COMBAT_PHASE_ON
    };
    
    enum MenuPhase
    {
        MENU_PHASE_ZOOM,
        MENU_PHASE_TRANSITION
    };
    
    //TODO: I despise this, change it 
    
    // MEMORY THAT SHOULDN'T BE CHANGED
    TextureId textureIdsByTag[(u32)AssetTag::COUNT];
    
    // MEMORY THAT YOU CAN RESET
    union {
        struct {
            
            Entity playerEId;
            EntityRegistry<ComponentList<KAMSKI_COMPONENTS>> entityRegistry;
            glm::vec3 camera;
            State gameState;
            ItemSet itemSet;
            bool isVroomOn;
            
            //TODO: save these to a config file
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
                KeyState hp;
                KeyState stam;
                KeyState mana;
                KeyState fastRestart;
            } actionState;
        };
        char disposableMemory[];
    };
    
    void gameInit();
    
    void gameInput();
    
    void gameUpdate();
    
    void gameRender();
    
    bool isCollision(glm::vec2 posA, glm::vec2 posB,
                     glm::vec2 sizeA, glm::vec2 sizeB)
    {
        return (abs(posA.x - posB.x) <= sizeA.x / 2.0f + sizeB.x / 2.0f) &&
        (abs(posA.y - posB.y) <= sizeA.y / 2.0f + sizeB.y / 2.0f);
    }
    
    //TODO: there is a bug here
    // fix it
    void renderMenuBackground()
    {
        // Textures to go through in the slideshow
        AssetTag tags[] = {
            AssetTag::SWORD_0,
            AssetTag::SHIELD_0,
            AssetTag::FORK_0
        };
        
        menuTime += deltaTime;
        ENGINE.addLight({}, 1.0f, glm::vec4(1.0f));
        switch(menuPhase)
        {
            case MENU_PHASE_ZOOM:
            {
                f32 zoom = (5.0f - (4.0f - menuTime)) / 5.0f;
                logDebug("zoom = %f", zoom);                
                ENGINE.drawTexturedQuad({}, 
                                        ENGINE.getScreenSize() * zoom, 
                                        getTextureIdByTag(tags[menuTagIndex]), 
                                        0);
                if(menuTime >= 4.0f)
                {
                    menuTime = 0.0f;
                    menuPhase = MENU_PHASE_TRANSITION;
                }
            }break;
            
            case MENU_PHASE_TRANSITION:
            {
                f32 zoom = (5.0f + menuTime) / 5.0f;
                f32 zoom2 = (5.0f - (8.0f - menuTime)) / 5.0f;
                f32 transition = (4.0f - menuTime) / 4.0f;
                ENGINE.drawQuad({},
                                ENGINE.getScreenSize() * zoom, 
                                getTextureIdByTag(tags[menuTagIndex]), 
                                glm::vec4(transition), 
                                0);
                
                ENGINE.drawQuad({},
                                ENGINE.getScreenSize() * zoom2, 
                                getTextureIdByTag(tags[(menuTagIndex + 1) % ARRAY_COUNT(tags)]), 
                                glm::vec4(1.0f - transition), 
                                0);
                
                if(menuTime >= 4.0f)
                {
                    menuTime = 0.0f;
                    menuPhase = MENU_PHASE_ZOOM;
                    menuTagIndex = (menuTagIndex + 1) % ARRAY_COUNT(tags);
                }
            }break;
            
        }
        
    }
    
    TextureId getTextureIdByItem(ItemType type, ItemBit bit)
    {
        AssetTag tag = AssetTag::NONE;
        switch (type)
        {
            case ITEM_TYPE_WEAPON:
            {
                tag = (AssetTag)((u32)AssetTag::WEAPON_START + bit);
            }break;
        }
        return getTextureIdByTag(tag);
    }
    
    void updateFollowers()
    {
        ComponentVector<FollowComponent>& followers = entityRegistry.getComponentVector<FollowComponent>();
        ComponentVector<TransformComponent>& transforms = entityRegistry.getComponentVector<TransformComponent>();
        
        for (Entity followerId: followers.iterateEntities())
        {
            const FollowComponent& follower = followers.getComponent(followerId);
            if (!entityRegistry.entityExists(follower.toFollowId))
            {
                entityRegistry.markEntityForDeletion(followerId);
                continue;
            }
            TransformComponent& followerTransform = transforms.getComponent(followerId);
            TransformComponent& toFollowTransform = transforms.getComponent(follower.toFollowId);
            followerTransform.position = toFollowTransform.position + follower.followOffset;
        }
        
        entityRegistry.removeMarkedEntities();
    }
    
    void updateHealthBars()
    {
        for (Entity healthBarId: entityRegistry.iterateEntities<HealthBarComponent, TransformComponent, FollowComponent>())
        {
            HealthBarComponent& healthBar = entityRegistry.getComponent<HealthBarComponent>(healthBarId);
            TransformComponent& healthBarComponent = entityRegistry.getComponent<TransformComponent>(healthBarId);
            FollowComponent& follower = entityRegistry.getComponent<FollowComponent>(healthBarId);
            if (!entityRegistry.hasComponent<EntityComponent>(follower.toFollowId))
            {
                entityRegistry.markEntityForDeletion(healthBarId);
                continue;
            }
            f32 healthPoints = entityRegistry.getComponent<EntityComponent>(follower.toFollowId).healthPoints;
            healthBarComponent.size.x = healthBar.maxSize * healthPoints / healthBar.maxHealth;
        }
        entityRegistry.removeMarkedEntities();
        
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
                    return " Shield:\n Reduces knockback from enemies by 80% ";
                    case WEAPON_FORK:
                    return " Fork:\n Attacks now shoot forks in the cursor's direction ";
                    case WEAPON_SWORD:
                    return " Sword:\n Attacks deal extra damage ";
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
    
    //TODO: figure out what the fuck this does
    void renderItems()
    {
        // Inventory
        glm::vec2 itemHolderBackgroundPos = ENGINE.uiTexture(glm::vec2{-260.0f, -200.0f}, glm::vec2{958.0f, 815.0f}, AnchorPoint::C, ID(ITEM_HOLDER_BACKGROUND));
        ENGINE.drawTextUI(glm::vec2{itemHolderBackgroundPos.x - 250.0f, itemHolderBackgroundPos.y + 450.0f}, 1200.0f, "Inventory");
        glm::vec2 inventorySlotPos[5*6];
        for(u32 y=0;y<5;y++)
            for(u32 x=0;x<6;x++)
        {
            inventorySlotPos[y*6 + x] = glm::vec2{itemHolderBackgroundPos.x - 375.0f + x*150.0f, itemHolderBackgroundPos.y + 300.0f - y*150.0f};
            ENGINE.drawUITex(inventorySlotPos[y*6 + x], glm::vec2{150, 150}, ID(ITEM_HOLDER_INVENTORY));
            
        }
        
        const EntityComponent& playerEntity = entityRegistry.getComponent<EntityComponent>(playerEId);
        char buffer[255];
        
        //Stats
        glm::vec2 statsBackgroundPos = ENGINE.uiTexture(glm::vec2{+500.0f, -200.0f}, glm::vec2{479.0f, 815.0f}, AnchorPoint::C, ID(ITEM_HOLDER_BACKGROUND));
        ENGINE.drawTextUI(glm::vec2{statsBackgroundPos.x - 100.0f, statsBackgroundPos.y + 450.0f}, 1200.0f, "Stats");
        sprintf(buffer, "Health: %.1f", playerEntity.healthPoints);
        ENGINE.drawTextUI(glm::vec2{statsBackgroundPos.x - 200.0f, statsBackgroundPos.y + 300.0f}, 500.0f, buffer);
        sprintf(buffer, "Speed: %.1f", playerEntity.movementSpeed);
        ENGINE.drawTextUI(glm::vec2{statsBackgroundPos.x - 200.0f, statsBackgroundPos.y + 300.0f - 75.0f*1}, 500.0, buffer);
        ENGINE.drawTextUI(glm::vec2{statsBackgroundPos.x - 200.0f, statsBackgroundPos.y + 300.0f - 75.0f*2}, 500.0f, "Stamina: ");
        sprintf(buffer, "Strength: %.1f", playerEntity.attackPoints);
        ENGINE.drawTextUI(glm::vec2{statsBackgroundPos.x - 200.0f, statsBackgroundPos.y + 300.0f - 75.0f*3}, 500.0f, buffer);
        ENGINE.drawTextUI(glm::vec2{statsBackgroundPos.x - 200.0f, statsBackgroundPos.y + 300.0f - 75.0f*4}, 500.0f, "Dexterity: ");
        ENGINE.drawUITex(glm::vec2{statsBackgroundPos.x, statsBackgroundPos.y - 250.0f}, TEXTURE_SIZES[ELF_M]*10.0f, ID(ELF_M_IDLE_0));
        
        glm::vec2 itemFrameSize = {100,100};
        u32 itemCount = 0;
        glm::vec2 buttonSize = {
            ENGINE.getScreenSize().x / 3.0f,
            ENGINE.getScreenSize().y / 6.5f
        };
        f32 fontSize = 500.0f;
        for (ItemBit i = 0; i != WEAPON_COUNT; i++)
        {
            if (hasWeapon(i))
            {
                if (ENGINE.uiButtonHover(inventorySlotPos[itemCount],
                                         itemFrameSize,
                                         AnchorPoint::C,
                                         getTextureIdByItem(ITEM_TYPE_WEAPON, i),
                                         getTextureIdByItem(ITEM_TYPE_WEAPON, i)))
                {
                    ENGINE.drawUITex(glm::vec2{cursorPosition.x + buttonSize.x/2, cursorPosition.y + buttonSize.y/2}, buttonSize, ID(BUTTON));
                    ENGINE.drawTextInsideBoxUI(glm::vec2{cursorPosition.x, cursorPosition.y + buttonSize.y},
                                               glm::vec2{cursorPosition.x + buttonSize.x, cursorPosition.y},
                                               fontSize,
                                               getItemDescription(ITEM_TYPE_WEAPON, i),
                                               30.0f);
                }
                itemCount++;
            }
        }
        
        for (ItemBit i = 0; i != ARMOUR_COUNT; i++)
        {
            if (hasArmour(i))
            {
                if (ENGINE.uiButtonHover(inventorySlotPos[itemCount],
                                         itemFrameSize,
                                         AnchorPoint::C,
                                         getTextureIdByItem(ITEM_TYPE_ARMOUR, i),
                                         getTextureIdByItem(ITEM_TYPE_ARMOUR, i)))
                {
                    ENGINE.drawUITex(glm::vec2{cursorPosition.x + buttonSize.x/2, cursorPosition.y + buttonSize.y/2}, buttonSize, ID(BUTTON));
                    ENGINE.drawTextInsideBoxUI(glm::vec2{cursorPosition.x, cursorPosition.y + buttonSize.y},
                                               glm::vec2{cursorPosition.x + buttonSize.x, cursorPosition.y},
                                               fontSize,
                                               getItemDescription(ITEM_TYPE_ARMOUR, i),
                                               30.0f);
                }
                itemCount++;
            }
        }
        
        for (ItemBit i = 0; i != UTILITY_COUNT; i++)
        {
            if (hasUtility(i))
            {
                if (ENGINE.uiButtonHover(inventorySlotPos[itemCount],
                                         itemFrameSize,
                                         AnchorPoint::C,
                                         getTextureIdByItem(ITEM_TYPE_UTILITY, i),
                                         getTextureIdByItem(ITEM_TYPE_UTILITY, i)))
                {
                    ENGINE.drawUITex(glm::vec2{cursorPosition.x + buttonSize.x/2, cursorPosition.y + buttonSize.y/2}, buttonSize, ID(BUTTON));
                    ENGINE.drawTextInsideBoxUI(glm::vec2{cursorPosition.x, cursorPosition.y + buttonSize.y},
                                               glm::vec2{cursorPosition.x + buttonSize.x, cursorPosition.y},
                                               fontSize,
                                               getItemDescription(ITEM_TYPE_UTILITY, i),
                                               30.0f);
                }
                itemCount++;
            }
        }
    }
    
    u32 hasWeapon(ItemBit weapon)
    {
        return itemSet.weapons & BIT(weapon);
    }
    
    u32 hasArmour(ItemBit armour)
    {
        return itemSet.armours & BIT(armour);
    }
    
    u32 hasUtility(ItemBit utility)
    {
        return itemSet.utility & BIT(utility);
    }
    
    void itemPickupSystem()
    {
        TransformComponent& playerSprite = entityRegistry.getComponent<TransformComponent>(playerEId);
        ColliderComponent& playerCollider = entityRegistry.getComponent<ColliderComponent>(playerEId);
        for (Entity eId : entityRegistry.iterateEntities<ItemComponent, TransformComponent>())
        {
            TransformComponent& itemSprite = entityRegistry.getComponent<TransformComponent>(eId);
            ColliderComponent& itemCollider = entityRegistry.getComponent<ColliderComponent>(eId);
            if (isCollision(playerSprite.position, itemSprite.position,
                            playerCollider.hitBox, itemCollider.hitBox))
            {
                ItemComponent& item = entityRegistry.getComponent<ItemComponent>(eId);
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
                entityRegistry.markEntityForDeletion(eId);
            }
        }
        
        entityRegistry.removeMarkedEntities();
    }
    
    void updatePlayerPosition()
    {
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
        
        TransformComponent& playerTransform = entityRegistry.getComponent<TransformComponent>(playerEId);
        EntityComponent& playerEntity = entityRegistry.getComponent<EntityComponent>(playerEId);
        VelocityComponent& v = entityRegistry.getComponent<VelocityComponent>(playerEId);
        v.targetVel = direction * playerEntity.movementSpeed;
        
        SpriteComponent& playerSprite = entityRegistry.getComponent<SpriteComponent>(playerEId);
        
        playerTransform.position = resolveBasePositionCollision(playerTransform.position - v.vel * (f32)deltaTime, playerTransform.position, ENTITY_TYPE_PLAYER);
        
        f32 len = glm::length(cursorPosition);
        len = fmax(len, 50);
        glm::vec2 offset = glm::normalize(cursorPosition) * len;
        
        camera.x = playerTransform.position.x + offset.x / 20.0f;
        camera.y = playerTransform.position.y + offset.y / 20.0f;
    }
    
    void updatePlayerAttack()
    {
        if (playerAttackTimer > 0.0)
        {
            playerAttackTimer -= deltaTime;
            return;
        }
        if (actionState.attack != KeyState::PRESS && actionState.attack != KeyState::HOLD)
        {
            return;
        }
        SpriteComponent& playerSprite = entityRegistry.getComponent<SpriteComponent>(playerEId);
        setAnimation(playerSprite, ELF_M_HIT);
        {
            playerAttackTimer = 0.4 - 0.3 * (f64)hasUtility(UTILITY_POTION);
            // cursorPosition is not affected by the camera position in calculations.
            //To convert cursorPosition to actual world Position you have to add the camera position to it
            EntityComponent& playerEntity = entityRegistry.getComponent<EntityComponent>(playerEId);
            
            Entity projectileId = entityRegistry.createEntity();
            glm::vec2 playerPos = entityRegistry.getComponent<TransformComponent>(playerEId).position;
            glm::vec2 directionVector = glm::normalize(cursorPosition + glm::vec2{camera.x, camera.y} - playerPos);
            
            entityRegistry.addComponent<ProjectileComponent>(projectileId, directionVector, 400.0f,
                                                             playerEntity.attackPoints + (50.0f * (f32)hasWeapon(WEAPON_SWORD)), false);
            
            entityRegistry.addComponent<TransformComponent>(projectileId, playerPos, TEXTURE_SIZES_WEAPONS[WEAPON_FORK],
                                                            (f32)(atan2(directionVector.y, directionVector.x) - PI / 2.0f));
            
            entityRegistry.addComponent<SpriteComponent>(projectileId, FORK);
        }
    }
    
    void renderSprites()
    {
        u32 cnt = 0;
        ComponentVector<SpriteComponent>& sprites = entityRegistry.getComponentVector<SpriteComponent>();
        Entity* entityIds = (Entity*)(ENGINE.temporaryAlloc(sprites.size() * sizeof(Entity)));
        
        for (Entity entityId: sprites.iterateEntities())
        {
            entityIds[cnt++] = entityId;
        }
        
        std::sort(entityIds, entityIds + cnt, [this](const Entity A, const Entity B)
                  {
                      const f32 aY = entityRegistry.getComponent<TransformComponent>(A).position.y;
                      const f32 bY = entityRegistry.getComponent<TransformComponent>(B).position.y;
                      return aY > bY;
                  });
        
        const SpriteComponent& playerSprite = entityRegistry.getComponent<SpriteComponent>(playerEId);
        TextureId playerTextureId = ENGINE.getAnimationFrame(playerSprite.animation, playerSprite.startTime);
        const TransformComponent& playerTransform = entityRegistry.getComponent<TransformComponent>(playerEId);
        
        for (u32 i = 0; i < cnt; ++i)
        {
            const SpriteComponent& entitySprite = entityRegistry.getComponent<SpriteComponent>(entityIds[i]);
            TextureId textureId = ENGINE.getAnimationFrame(entitySprite.animation, entitySprite.startTime);
            TransformComponent entityTransform = entityRegistry.getComponent<TransformComponent>(entityIds[i]);
            // draw texture
            ENGINE.drawTexturedQuad(
                                    entityTransform.position,
                                    ENTITIES_SIZE_MULTIPLIER * entityTransform.size,
                                    textureId,
                                    entityTransform.rotation
                                    );
        }
        
        for (Entity colorId: entityRegistry.iterateEntities<SolidColorComponent, TransformComponent>())
        {
            TransformComponent& colorTransform = entityRegistry.getComponent<TransformComponent>(colorId);
            glm::vec4 color = entityRegistry.getComponent<SolidColorComponent>(colorId).color;
            ENGINE.drawColoredQuad(colorTransform.position, colorTransform.size, color, colorTransform.rotation);
        }
        
        const EntityComponent& playerEntity = entityRegistry.getComponent<EntityComponent>(playerEId);
        f32 maxBarLength = 923.0f;
        f32 healthBarLength = maxBarLength/200.0f*playerEntity.healthPoints;
        
        // Stats bars
        {
            glm::vec2 hudBarsPos = {0.0f,0.0f};
            hudBarsPos = ENGINE.uiTexture(glm::vec2{20,-20}, glm::vec2{1218, 196}, AnchorPoint::NW, ID(STATS_UI));
            ENGINE.drawQuadUI(glm::vec2{hudBarsPos.x + 72.0f - (maxBarLength - healthBarLength)/2, hudBarsPos.y + 64.0f}, glm::vec2{healthBarLength,22}, glm::vec4{0.9f,0.1f,0.3f,0.8f}, 0.0f);
            ENGINE.drawQuadUI(glm::vec2{hudBarsPos.x + 72.0f, hudBarsPos.y + 0.0f}, glm::vec2{920,22}, glm::vec4{0.1f,0.9f,0.3f,0.8f}, 0.0f);
            ENGINE.drawQuadUI(glm::vec2{hudBarsPos.x - 238.0f, hudBarsPos.y + -64.0f}, glm::vec2{305,22}, glm::vec4{0.4f,0.4f,0.9f,0.8f}, 0.0f);
        }
        
        // Quick access items
        {
            glm::vec2 quickItemsSlotPos[8];
            glm::vec2 hudItemHolderPos = ENGINE.uiTexture(glm::vec2{20, 50}, glm::vec2{45, 84}, AnchorPoint::SW, ID(ITEM_HOLDER_START));
            u32 i=0;
            for(i=0;i<3;i++)
            {
                quickItemsSlotPos[i] = glm::vec2{195.0f/2 + hudItemHolderPos.x + i*150, hudItemHolderPos.y};
                ENGINE.drawUITex(quickItemsSlotPos[i], glm::vec2{150, 144}, ID(ITEM_HOLDER)); 
            }
            ENGINE.drawUITex(glm::vec2{45.0f + hudItemHolderPos.x + i*150, hudItemHolderPos.y}, glm::vec2{45, 84}, ID(ITEM_HOLDER_END));
        }
        
        //items
        {
            ENGINE.drawUITex(quickItemsSlotPos[0], glm::vec2{120,120}, ID(HEALTH_POTION));
            ENGINE.drawUITex(quickItemsSlotPos[1], glm::vec2{120,120}, ID(STAMINA_POTION));
            ENGINE.drawUITex(quickItemsSlotPos[2], glm::vec2{120,120}, ID(MANA_POTION));
        }    
    }
    
    void velocitySystem()
    {
        for (Entity vEid: entityRegistry.iterateEntities<VelocityComponent>())
        {
            VelocityComponent& v = entityRegistry.getComponent<VelocityComponent>(vEid);
            TransformComponent& t = entityRegistry.getComponent<TransformComponent>(vEid);
            v.vel += (v.targetVel - v.vel) * 20.0f * (f32)deltaTime;
            t.position += v.vel * (f32)deltaTime;
        }
    }
    
    void startGame()
    {
        initMap(MAP_SIZE_X, MAP_SIZE_Y);
        // ENGINE.printGlobalAllocations(false);
        gameState = GAME_RUNNING;
    }
    
    bool isOnScreen(glm::vec2 pos) 
    {
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
    
    void initMap(const u32 sizeX, const u32 sizeY)
    {
        logError("Map generation not yet implemented!");
    }
    
    void renderMap() const
    {
        for (u32 i = 0; i < map.tilesArrSize; ++i)
        {
            glm::uvec2* t = &map.tilesArr[i];
            glm::uvec2 tile = map.tilesArr[i];
            glm::vec2 position = getCenterPositionByTile(tile);
            u32 tileIndex = tile.y * map.size.x + tile.x;
            ENGINE.drawTexturedQuad(position, map.quadSize, getTextureIdByTag(map.tiles[tileIndex]), 0);
        }
        for (u32 i = 0; i < map.numberOfWalls; ++i)
        {
            glm::vec2 center = (map.walls[i].leftBottom + map.walls[i].topRight) / 2.0f;
            glm::vec2 size = map.walls[i].topRight - map.walls[i].leftBottom;
            ENGINE.addLightBlocker(center, size);
        }
        
    }
    
    glm::vec2 getQuadSize() const
    {
        return map.quadSize;
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
    GAME->deltaTime = deltaTime;
    GAME->gameUpdate();
    deltaTime = GAME->deltaTime;
}

extern "C"
__declspec(dllexport)
void gameRender(f64 deltaTime) {
    GAME->gameRender();
}
