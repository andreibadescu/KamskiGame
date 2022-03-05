#include "KamskiEngine/KamskiRenderer.h"
#include "KamskiEngine/KamskiIO.h"
#include "KamskiEngine/KamskiMemory.h"
#include "KamskiEngine/KamskiLog.h"

extern "C"
__declspec(dllexport)
void gameInput(const GameLog& logger, const GameIO& input, void* const inputState)
{
    logInfo("Test1");
}

extern "C"
__declspec(dllexport)
void gameUpdate(const GameLog& logger, void* const gameState, const void* const inputState, GameMemory& memory)
{
    logInfo("Test2");
}

extern "C"
__declspec(dllexport)
void gameRender(const GameLog& logger, GameRenderer& renderer, const void* const gameState, GameMemory& memory)
{
    logInfo("Test3");
}
