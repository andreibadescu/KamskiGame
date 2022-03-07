#include "KamskiEngine/KamskiRenderer.h"
#include "KamskiEngine/KamskiIO.h"
#include "KamskiEngine/KamskiMemory.h"
#include "KamskiEngine/KamskiLog.h"

#include "headers/GameState.h"
#include "headers/InputState.h"

extern "C"
__declspec(dllexport)
void gameInit(const GameLog& logger, GameState& gameState, const GameRenderer& renderer, GameMemory& memory)
{
    gameState.map.init(MAP_SIZE_X, MAP_SIZE_Y);
    gameState.setDrawFunction(renderer.drawQuad);

    for (u8 i = 0; i < TEXTURE_COUNT; ++i)
    {
        const texture_id textureId = static_cast<texture_id>(renderer.loadTexture(TEXTURE_PATHS[i]));
        GameState::linkTextureIdByTag(textureId, static_cast<TextureTag>(i));
    }

    gameState.addPlayer(0, 0.0f, 0.0f, GameState::getTextureIdByTag(TextureTag::PLAYER), 0.4f, 500, 50);
    gameState.addEnemy(1, -0.5f, -0.5f, GameState::getTextureIdByTag(TextureTag::ENEMY), 0.2f, 200, 20);
    gameState.addEnemy(2, 0.5f, -0.5f, GameState::getTextureIdByTag(TextureTag::ENEMY), 0.2f, 200, 20);
    gameState.addEnemy(3, -0.5f, 0.5f, GameState::getTextureIdByTag(TextureTag::ENEMY), 0.2f, 200, 20);
    gameState.addEnemy(4, 0.5f, 0.5f, GameState::getTextureIdByTag(TextureTag::ENEMY), 0.2f, 200, 20);

    gameState.map.load();

    logInfo("[+] Init - done!");
}

extern "C"
__declspec(dllexport)
void gameInput(const GameLog& logger, const GameIO& input, GameState& gameState)
{
    gameState.input.setUp(input.getKeyState('W') == KeyState::HOLD || input.getKeyState('W') == KeyState::PRESS);
    gameState.input.setLeft(input.getKeyState('A') == KeyState::HOLD || input.getKeyState('A') == KeyState::PRESS);
    gameState.input.setDown(input.getKeyState('S') == KeyState::HOLD || input.getKeyState('S') == KeyState::PRESS);
    gameState.input.setRight(input.getKeyState('D') == KeyState::HOLD || input.getKeyState('D') == KeyState::PRESS);
}

extern "C"
__declspec(dllexport)
void gameUpdate(const GameLog& logger, GameState& gameState, GameMemory& memory, f64& deltaTime)
{
    gameState.updatePlayer(static_cast<f32>(deltaTime));
    gameState.updateEnemies(static_cast<f32>(deltaTime));
}

extern "C"
__declspec(dllexport)
void gameRender(const GameLog& logger, const GameRenderer& renderer, const GameState& gameState, GameMemory& memory, const f64 deltaTime)
{
    renderer.beginBatch();
    gameState.map.render(renderer.drawQuad);
    gameState.renderSprites();
    renderer.endBatch();
}
