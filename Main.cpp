#include "KamskiEngine/KamskiRenderer.h"
#include "KamskiEngine/KamskiIO.h"
#include "KamskiEngine/KamskiMemory.h"
#include "KamskiEngine/KamskiLog.h"
#include "KamskiEngine/KamskiContainers.h"
#include <cmath>

#define MAP_SIZE 20

enum class TextureTag
{
    FLOOR,
    WALL_RIGHT,
    WALL_LEFT,
    WALL_TOP,
    WALL_BOTTOM,
    WALL_CORNER_LEFT_BOTTOM,
    WALL_CORNER_LEFT_TOP,
    WALL_CORNER_RIGHT_BOTTOM,
    WALL_CORNER_RIGHT_TOP,
    PLAYER,
    MAX_ENUM
};

const char* texturePaths[] = 
{
    "assets/floor.png",  
    "assets/wall_right.png",  
    "assets/wall_left.png",  
    "assets/wall_mid.png",  
    "assets/wall_bottom_mid.png",
    "assets/wall_corner_bottom_left.png",  
    "assets/wall_corner_top_left.png",  
    "assets/wall_corner_bottom_right.png",  
    "assets/wall_corner_top_right.png",  
    "assets/player.jpg"  
};

struct Sprite
{
    f32 x, y;
    u32 texId;
};

struct GameState
{
    u32 map[MAP_SIZE][MAP_SIZE];
    u32 textureIds[(u32)TextureTag::MAX_ENUM];
    glm::vec2 playerPos;
    ComponentVector<Sprite> spriteComponents;
    Entity playerId;
    Entity enemyId;
};

struct InputState
{
    KeyState walkForward;
    KeyState walkDownward;
    KeyState walkRight;
    KeyState walkLeft;
};

extern "C"
__declspec(dllexport)
void gameInit(const GameLog& logger, void* inputState, GameState& gameState, GameRenderer& renderer, GameMemory& memory)
{
    
    gameState.playerId = 0;
    gameState.enemyId = 1;
    for(u32 i = 0; i < ARRAY_COUNT(texturePaths); i++)
    {
        gameState.textureIds[i] = renderer.loadTexture(texturePaths[i]);
    }
    
    for(u32 i = 1; i < MAP_SIZE - 1; i++)
    {
        gameState.map[i][0] = gameState.textureIds[(u32)TextureTag::WALL_LEFT];
        gameState.map[i][MAP_SIZE - 1] = gameState.textureIds[(u32)TextureTag::WALL_RIGHT];
        gameState.map[0][i] = gameState.textureIds[(u32)TextureTag::WALL_BOTTOM];
        gameState.map[MAP_SIZE - 1][i] = gameState.textureIds[(u32)TextureTag::WALL_TOP];
    }
    
    gameState.map[0][0] = gameState.textureIds[(u32)TextureTag::WALL_CORNER_LEFT_BOTTOM];
    gameState.map[MAP_SIZE - 1][0] = gameState.textureIds[(u32)TextureTag::WALL_CORNER_LEFT_TOP];
    gameState.map[0][MAP_SIZE - 1] = gameState.textureIds[(u32)TextureTag::WALL_CORNER_RIGHT_BOTTOM];
    gameState.map[MAP_SIZE - 1][MAP_SIZE - 1] = gameState.textureIds[(u32)TextureTag::WALL_CORNER_RIGHT_TOP];
    
    for(u32 i = 1; i < MAP_SIZE - 1; i++)
    {
        for(u32 j = 1; j < MAP_SIZE - 1; j++)
        {
            gameState.map[i][j] = gameState.textureIds[(u32)TextureTag::FLOOR];
        }
    }
    gameState.spriteComponents.addComponent(gameState.playerId, Sprite{0,0, gameState.textureIds[(u32)TextureTag::PLAYER]});
}



extern "C"
__declspec(dllexport)
void gameInput(const GameLog& logger, const GameIO& input, InputState& inputState, void* gameState, GameMemory& memory)
{
    inputState.walkForward = input.getKeyState('W');
    inputState.walkDownward= input.getKeyState('S');
    inputState.walkRight = input.getKeyState('D');
    inputState.walkLeft = input.getKeyState('A');
}

extern "C"
__declspec(dllexport)
void gameUpdate(const GameLog& logger, GameState& gameState, const InputState& inputState, GameMemory& memory, f64& dt)
{
    //logInfo("dt:%f", dt);
    Sprite& playerSprite = gameState.spriteComponents.getComponent(gameState.playerId); //ew improve interface
    f32 speed = 0.2f;
    if(inputState.walkForward == KeyState::HOLD)
    {
        playerSprite.y += speed * dt;
    }
    
    if(inputState.walkDownward == KeyState::HOLD)
    {
        playerSprite.y -= speed * dt;
    }
    
    if(inputState.walkRight == KeyState::HOLD)
    {
        playerSprite.x += speed * dt;
    }
    
    if(inputState.walkLeft== KeyState::HOLD)
    {
        playerSprite.x -= speed * dt;
    }
    
}

extern "C"
__declspec(dllexport)
void gameRender(const GameLog& logger, GameRenderer& renderer, const GameState& gameState, GameMemory& memory, const f64 dt)
{
    renderer.beginBatch();
    const f32 tileSize = 2.0f / (f32)MAP_SIZE;
    for(u32 i = 0; i < MAP_SIZE; i++)
    {
        for(u32 j = 0; j < MAP_SIZE; j++)
        {
            glm::vec2 pos = glm::vec2(j * tileSize, i * tileSize) + glm::vec2(tileSize) / 2.0f - 1.0f;
            renderer.drawQuad(pos, {tileSize, tileSize}, gameState.map[i][j]);
        }
    }
    
    for(const Sprite& sp : gameState.spriteComponents.iterateComponents())
    {
        renderer.drawQuad({sp.x, sp.y}, {tileSize, tileSize}, sp.texId);
    }
    
    renderer.endBatch();
    
}
