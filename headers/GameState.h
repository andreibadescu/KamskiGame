#pragma once

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

    void addPlayer(Entity eId, glm::vec2 position, TextureTag tag,
                   f32 movementSpeed, f32 healthPoints, f32 attackPoints);

    void addEnemy(Entity eId, glm::vec2 position, TextureTag tag,
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

private:
    texture_id textureIdsByTag[static_cast<u32>(TextureTag::COUNT)];
    bool gameOver;
    f32 deltaTime;

    ComponentVector<SpriteComponent> sprites;
    ComponentVector<SolidColorComponent> solidColors;
    ComponentVector<EntityComponent> players;
    ComponentVector<EntityComponent> enemies;
    ComponentVector<ProjectileComponent> projectiles;
    ComponentVector<FollowComponent> followers;
    ComponentVector<HealthBarComponent> healthBars;

    static bool isCollision(const SpriteComponent& A, const SpriteComponent& B);

    void updatePlayerPosition();

    void updatePlayerHealth();

    void updatePlayerAttack();
};
