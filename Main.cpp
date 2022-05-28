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
    logDebug("%f %f", buttonSize.x, buttonSize.y);
    for (u32 i = 0; i < TEXTURE_COUNT; ++i)
    {
        const texture_id textureId = ENGINE.loadTexture(TEXTURE_PATHS[i]);
        linkTextureIdByTag(textureId, static_cast<TextureTag>(i));
    }
    GAME->gameState = MAIN_MENU;
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

void Game::gameUpdate()
{
    switch (gameState)
    {
    case MAIN_MENU:
        ENGINE.setBlurWholeScreen(false);
        if (menuButton({0.0f, -ENGINE.getScreenSize().y / 5.0f}, "EXIT"))
        {
            ENGINE.exit(EXIT_SUCCESS);
        }
        else if (menuButton({0.0f, 0.0f}, "SETTINGS"))
        {
            gameState = SETTINGS;
        }
        else if (menuButton({0.0f, +ENGINE.getScreenSize().y / 5.0f}, "PLAY"))
        {
            gameState = GAME_START;
        }
        break;

    case SETTINGS:
        if (menuButton({0.0f, 0.0f}, "GO BACK"))
        {
            gameState = MAIN_MENU;
        }
        break;

    case GAME_PAUSED:
        if (actionState.pauseGame == KeyState::PRESS)
        {
            gameState = GAME_RUNNING;
        }
        break;

    [[unlikely]]
    case GAME_START:
        ENGINE.setBlurWholeScreen(false);
        memset((char*)this + offsetof(Game, disposableMemory),
                0,
                sizeof(Game) - offsetof(Game, disposableMemory));
        startGame();
        break;

    [[likely]]
    case GAME_RUNNING:
        if (actionState.pauseGame == KeyState::PRESS)
        {
            gameState = GAME_PAUSED;
        }

        // check if player pressed RESTART game
        if (actionState.restart == KeyState::PRESS)
        {
            ENGINE.globalFree(map.tiles);
            gameState = GAME_LOST;
        }
        else
        {
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
        ENGINE.setBlurWholeScreen(true);
        if (menuButton({0.0f, -ENGINE.getScreenSize().y / 5.0f}, "EXIT"))
        {
            ENGINE.exit(EXIT_SUCCESS);
        }
        else if (menuButton({0.0f, 0.0f}, "GO TO MENU"))
        {
            gameState = MAIN_MENU;
        }
        else if (menuButton({0.0f, +ENGINE.getScreenSize().y / 5.0f}, "PLAY AGAIN"))
        {
            gameState = GAME_START;
        }
        break;
    }
}

void Game::gameRender()
{
    ENGINE.beginBatch(camera);
    switch (gameState) {
        case GAME_RUNNING:
        case GAME_LOST:
            render();
            renderSprites();
            break;
    }
    ENGINE.endBatch();
    ENGINE.swapClear();
}
