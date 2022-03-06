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
    logInfo("Test - Init");
}

extern "C"
__declspec(dllexport)
void gameInput(const GameLog& logger, const GameIO& input, InputState& inputState)
{
    logInfo("Test - Input");
}

extern "C"
__declspec(dllexport)
void gameUpdate(const GameLog& logger, GameState& gameState, const InputState& inputState, GameMemory& memory, f64& deltaTime)
{
    logInfo("Test - Update");
}

extern "C"
__declspec(dllexport)
void gameRender(const GameLog& logger, GameRenderer& renderer, const GameState& gameState, GameMemory& memory, const f64 deltaTime)
{
    logInfo("Test - Renderer");
}
