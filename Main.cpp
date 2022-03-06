#include "KamskiEngine/KamskiRenderer.h"
#include "KamskiEngine/KamskiIO.h"
#include "KamskiEngine/KamskiMemory.h"
#include "KamskiEngine/KamskiLog.h"

#include "headers/GameState.h"
#include "headers/InputState.h"

extern "C"
__declspec(dllexport)
void gameInit(const GameLog& logger, InputState& inputState, GameState& gameState, GameRenderer& renderer, GameMemory& memory)
{
    gameState.map.init(MAP_SIZE_X, MAP_SIZE_Y);

    for (u8 i = 0; i < TEXTURE_COUNT; ++i)
    {
        const texture_id textureId = static_cast<u8>(renderer.loadTexture(TEXTURE_PATHS[i]));
        gameState.map.linkTextureTagAndId(textureId, static_cast<TextureTag>(i));
    }

    gameState.map.load();

    logInfo("[+] Init - done!");
}

extern "C"
__declspec(dllexport)
void gameInput(const GameLog& logger, const GameIO& input, InputState& inputState)
{
}

extern "C"
__declspec(dllexport)
void gameUpdate(const GameLog& logger, GameState& gameState, const InputState& inputState, GameMemory& memory, f64& deltaTime)
{
}

extern "C"
__declspec(dllexport)
void gameRender(const GameLog& logger, GameRenderer& renderer, const GameState& gameState, GameMemory& memory, const f64 deltaTime)
{
    renderer.beginBatch();
    gameState.map.render(renderer.drawQuad);
    renderer.endBatch();
}
