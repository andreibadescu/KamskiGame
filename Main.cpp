#include "headers/Defines.h"
#include "GameState.cpp"

extern "C"
__declspec(dllexport)
void loadEngine(const API& api)
{
    /* INIT ENGINE SUBSYSTEMS */
    GAME_STATE = (GameState*)api.gameState;
    ENGINE = api;
}

extern "C"
__declspec(dllexport)
void gameInit()
{
    for (u32 i = 0; i < TEXTURE_COUNT; ++i)
    {
        const texture_id textureId = ENGINE.loadTexture(TEXTURE_PATHS[i]);
        GAME_STATE->linkTextureIdByTag(textureId, static_cast<TextureTag>(i));
    }
    GAME_STATE->startGame();
    logInfo("[+] Init - done!");
}

extern "C"
__declspec(dllexport)
void gameInput()
{
    // set action keys
    GAME_STATE->actionState.startGame = ENGINE.getKeyState('R');
    GAME_STATE->actionState.walkUp = ENGINE.getKeyState('W');
    GAME_STATE->actionState.walkLeft = ENGINE.getKeyState('A');
    GAME_STATE->actionState.walkDown = ENGINE.getKeyState('S');
    GAME_STATE->actionState.walkRight = ENGINE.getKeyState('D');
    GAME_STATE->actionState.zoomIn = ENGINE.getKeyState('Q');
    GAME_STATE->actionState.zoomOut = ENGINE.getKeyState('E');
    GAME_STATE->actionState.pauseGame = ENGINE.getKeyState('P');
    GAME_STATE->actionState.attack = ENGINE.getSpecialKeyState(SpecialKeyCode::SPACE);
    GAME_STATE->actionState.restart = ENGINE.getSpecialKeyState(SpecialKeyCode::RETURN);

    if (ENGINE.getSpecialKeyState(SpecialKeyCode::CTRL) == KeyState::PRESS)
    {
        GAME_STATE->playerToggleVroom();
    }
    // set cursor position
    ENGINE.getMousePosition(GAME_STATE->cursorPosition.x, GAME_STATE->cursorPosition.y);
}

extern "C"
__declspec(dllexport)
void gameUpdate(f64& deltaTime)
{
    if (!GAME_STATE->gameHasStarted())
    {
        return;
    }
    static bool paused = false;
    if (GAME_STATE->actionState.pauseGame == KeyState::PRESS)
    {
        paused = !paused;
    }

    deltaTime = deltaTime * (f64)(!paused);
    // check if player pressed RESTART game
    if (GAME_STATE->actionState.restart == KeyState::PRESS)
    {
        GAME_STATE->stopGame();
    }
    else
    {
        GAME_STATE->updateDeltaTime(static_cast<f32>(deltaTime));
        // update entities
        GAME_STATE->updatePlayer();
        GAME_STATE->updateEnemies();
        GAME_STATE->moveProjectiles();
        GAME_STATE->updateFollowers();
        GAME_STATE->updateHealthBars();
    }

    if (GAME_STATE->playerHasDied())
    {
        memset(GAME_STATE, 0, sizeof(*GAME_STATE));
        gameInit();
    }
}

extern "C"
__declspec(dllexport)
void gameRender(const f64 deltaTime)
{
    glm::vec3 cam = GAME_STATE->getCamera();
    ENGINE.beginBatch(cam.x, cam.y, cam.z);
    GAME_STATE->render();
    GAME_STATE->renderSprites();
    ENGINE.endBatch();
}
