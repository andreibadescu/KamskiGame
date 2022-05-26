#pragma once

#include "Defines.h"
#include "Components.h"

class Game
{
public:
    Entity playerEId;
    texture_id textureIdsByTag[(u32)TextureTag::COUNT];
    f32 deltaTime;
    EntityRegistry<ComponentList<KAMSKI_COMPONENTS>> entityRegistry;
    glm::vec3 camera;
    bool isVroomOn;
    glm::vec2 startPosition;
    f32 playerRadius;
    animation_id* animations;
    States gameState;

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
    } actionState;

    glm::vec2 cursorPosition;
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
    } map;

    Game() = delete;

    void gameInit();

    void gameInput();

    void gameUpdate(f64& deltaTime);

    void gameRender(const f64 deltaTime);

    void linkTextureIdByTag(texture_id id, TextureTag tag);

    texture_id getTextureIdByTag(TextureTag tag) const;

    void addPlayer(glm::vec2 position, TextureTag tag, f32 movementSpeed, f32 healthPoints, f32 attackPoints);

    void addPlayer(glm::vec2 position);

    Entity addEntity(glm::vec2 position, TextureTag tag, f32 movementSpeed, f32 healthPoints, f32 attackPoints);

    Entity addEntity(glm::vec2 position, EntityStats stats);

    Entity addEntity(glm::vec2 position, EntityType enemyType);

    void updateFollowers();

    void updateHealthBars();

    void updatePlayer();

    void updateEnemies();

    void renderSprites() const;

    void startGame();

    void stopGame();

    void updateDeltaTime(f32 deltaTime);

    void moveProjectiles();

    void changeGameState();

    bool gameHasStarted() const;

    EntityRegistry<ComponentList<KAMSKI_COMPONENTS>>& getECS();

    glm::vec3 getCamera() const;

    glm::vec2 getCameraUnits() const;

    glm::vec2 getCameraLeftTopCorner() const;

    glm::vec2 getCameraRightTopCorner() const;

    glm::vec2 getCameraLeftBottomCorner() const;

    glm::vec2 getCameraRightBottomCorner() const;

    bool isOnScreen(glm::vec2 pos) const;

    static bool isCollision(const SpriteComponent& A, const SpriteComponent& B);

    void updatePlayerPosition();

    void updatePlayerHealth();

    void updatePlayerAttack();

    void playerToggleVroom();

    glm::uvec2 getTileByPosition(glm::vec2 position) const;

    glm::vec2 getPlayerSpritePosition() const;

    glm::vec2 getPlayerBasePosition() const;

    glm::vec2 getBasePosition(glm::vec2 spritePos, TextureTag tag) const;

    glm::vec2 playerBaseToSpritePosition(glm::vec2 basePosition) const;

    glm::vec2 baseToSpritePosition(glm::vec2 basePosition, TextureTag tag) const;

    f32 calcRadiusOfEntity(TextureTag tag);

    void resolveCollision(glm::vec2 oldPos, glm::vec2& pos, u32 radius);

    glm::vec2 resolveBasePositionCollision(glm::vec2 oldPosition, glm::vec2 position, TextureTag tag);

    void init(u32 sizeX, u32 sizeY);

    void render() const;

    glm::vec2 getQuadSize() const;

    void generateMapLevel();

    glm::vec2 getCenterPositionByTile(glm::uvec2 tile) const;

    glm::vec2 getLeftBottomCornerByTile(glm::uvec2 tile) const;

    bool checkFillTile(bool* mat, u32 tileIndex) const;

    bool fill_util_x(bool* mat, i32 tileX, i32 tileY);

    bool fill_util_y(bool* mat, i32 tileX, i32 tileY);

    void fill();
};
