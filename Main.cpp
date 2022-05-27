#include "Game.cpp"

extern "C"
__declspec(dllexport)
void loadEngine(const API& api)
{
    /* INIT ENGINE SUBSYSTEMS */
    GAME = (Game*)api.gameState;
    ENGINE = api;
}

void Game::gameInit()
{
    for (u32 i = 0; i < TEXTURE_COUNT; ++i)
    {
        const texture_id textureId = ENGINE.loadTexture(TEXTURE_PATHS[i]);
        linkTextureIdByTag(textureId, static_cast<TextureTag>(i));
    }
    startGame();
    logInfo("[+] Init - done!");
}

void Game::gameInput()
{
    // set action keys
    actionState.startGame = ENGINE.getKeyState('R');
    actionState.walkUp = ENGINE.getKeyState('W');
    actionState.walkLeft = ENGINE.getKeyState('A');
    actionState.walkDown = ENGINE.getKeyState('S');
    actionState.walkRight = ENGINE.getKeyState('D');
    actionState.zoomIn = ENGINE.getKeyState('Q');
    actionState.zoomOut = ENGINE.getKeyState('E');
    actionState.pauseGame = ENGINE.getKeyState('P');
    actionState.attack = ENGINE.getSpecialKeyState(SpecialKeyCode::SPACE);
    actionState.restart = ENGINE.getSpecialKeyState(SpecialKeyCode::RETURN);

    if (ENGINE.getSpecialKeyState(SpecialKeyCode::CTRL) == KeyState::PRESS)
    {
        playerToggleVroom();
    }
    // set cursor position
    ENGINE.getMousePosition(cursorPosition.x, cursorPosition.y);
}

void Game::gameUpdate(f64& deltaTime)
{
    switch (gameState)
    {
    case MAIN_MENU:
        break;

    case GAME_PAUSED:
        if (actionState.pauseGame == KeyState::PRESS)
        {
            gameState = GAME_RUNNING;
        }
        break;

    [[unlikely]]
    case GAME_START:
        memset(this, 0, sizeof(Game));
        gameInit();
        break;

    [[likely]]
    case GAME_RUNNING:
        if (actionState.pauseGame == KeyState::PRESS)
        {
            gameState = GAME_PAUSED;
        }
        if(ENGINE.uiButtonHoverText(
                                    {0,0},
                                    ENGINE.getScreenSize() / 10.0f,
                                    AnchorPoint::C,
                                    ID(BUTTON),
                                    ID(BUTTON_PRESSED),
                                    10.0f,
                                    "PLAY AGAIN"))
        {
            gameState = GAME_START;
        }            
        
        deltaTime = deltaTime;
        // check if player pressed RESTART game
        if (actionState.restart == KeyState::PRESS)
        {
            ENGINE.globalFree(map.tiles);
            gameState = GAME_LOST;
        }
        else
        {
            updateDeltaTime((f32)deltaTime);
            velocitySystem();
            updatePlayer();
            updateEnemies();
            moveProjectiles();
            updateFollowers();
            updateHealthBars();
            itemPickupSystem();
        }
        break;

    [[unlikely]]
    case GAME_LOST:
        break;
    }
}

void Game::gameRender(const f64 deltaTime)
{
    glm::vec3 cam = getCamera();
    ENGINE.beginBatch(cam.x, cam.y, cam.z);
    render();
    renderSprites();
    ENGINE.endBatch();
}
