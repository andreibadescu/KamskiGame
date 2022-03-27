#pragma once

#include "Defines.h"
#include "KamskiEngine/KamskiContainers.h"
#include "KamskiEngine/KamskiIO.h"

#include "Map.h"
#include "Components.h"

class GameState
{
public:
    struct
    {
        KeyState startGame;
        KeyState walkUp;
        KeyState walkDown;
        KeyState walkLeft;
        KeyState walkRight;
        KeyState attack;
        KeyState restart;
    } actionState;
    glm::vec2 cursorPosition;
    Map map;

    GameState() = delete;

    bool playerHasDied() const;

    void linkTextureIdByTag(texture_id id, TextureTag tag);

    [[nodiscard]]
    texture_id getTextureIdByTag(TextureTag tag) const;

    void addPlayer(glm::vec2 position, TextureTag tag,
                   f32 movementSpeed, f32 healthPoints, f32 attackPoints);

    void addEnemy(glm::vec2 position, TextureTag tag,
                  f32 movementSpeed, f32 healthPoints, f32 attackPoints);

    void updateFollowers();

    void updateHealthBars();

    void updatePlayer();

    void updateEnemies();

    void renderSprites() const;

    void startGame();

    void stopGame();

    void updateDeltaTime(f32 deltaTime);

    void moveProjectiles();

    bool gameHasStarted() const;

    void initECS();

    EntityRegistry<ComponentList<KAMSKI_COMPONENTS>>& getECS();

private:
    Entity playerEId;
    texture_id textureIdsByTag[static_cast<u32>(TextureTag::COUNT)];
    bool gameOver;
    f32 deltaTime;

    EntityRegistry<ComponentList<KAMSKI_COMPONENTS>> entityRegistry;

    static bool isCollision(const SpriteComponent& A, const SpriteComponent& B);

    void updatePlayerPosition();

    void updatePlayerHealth();

    void updatePlayerAttack();
};
