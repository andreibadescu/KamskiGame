#include "headers/Defines.h"
#include "headers/GameState.h"

extern "C"
__declspec(dllexport)
void loadEngine(const GameLog& logger, GameState& gameState, const GameRenderer& renderer, GameMemory& memory)
{
    /* INIT ENGINE SUBSYSTEMS */
    LOGGER = &logger;
    GAME_STATE = &gameState;
    RENDERER = &renderer;
    MEMORY = &memory;
}

extern "C"
__declspec(dllexport)
void gameInit()
{
    GAME_STATE->initECS();
    GAME_STATE->map.init(MAP_SIZE_X, MAP_SIZE_Y);

    for (u8 i = 0; i < TEXTURE_COUNT; ++i)
    {
        const texture_id textureId = RENDERER->loadTexture(TEXTURE_PATHS[i]);
        GAME_STATE->linkTextureIdByTag(textureId, static_cast<TextureTag>(i));
    }

    GAME_STATE->addPlayer({ 0.0f, 0.0f }, TextureTag::PLAYER, 500.0f, 200, 50);
    GAME_STATE->addEnemy({-500.0f, -500.0f}, TextureTag::ENEMY, 200.0f, 200, 25);
    GAME_STATE->addEnemy({ 500.0f, -500.0f }, TextureTag::ENEMY, 200.0f, 200, 25);
    GAME_STATE->addEnemy({ -500.0f, 500.0f }, TextureTag::ENEMY, 200.0f, 200, 25);
    GAME_STATE->addEnemy({ 500.0f, 500.0f }, TextureTag::ENEMY, 200.0f, 200, 25);

    GAME_STATE->map.load();
    GAME_STATE->stopGame();
    GAME_STATE->updateFollowers();
    logInfo("[+] Init - done!");
}

extern "C"
__declspec(dllexport)
void gameInput(const GameIO& input)
{
    // set action keys
    GAME_STATE->actionState.startGame = input.getKeyState('R');
    GAME_STATE->actionState.walkUp = input.getKeyState('W');
    GAME_STATE->actionState.walkLeft = input.getKeyState('A');
    GAME_STATE->actionState.walkDown = input.getKeyState('S');
    GAME_STATE->actionState.walkRight = input.getKeyState('D');
    GAME_STATE->actionState.zoomIn = input.getKeyState('Q');
    GAME_STATE->actionState.zoomOut = input.getKeyState('E');
    GAME_STATE->actionState.pauseGame = input.getKeyState('P');
    GAME_STATE->actionState.attack = input.getSpecialKeyState(SpecialKeyCode::SPACE);
    GAME_STATE->actionState.restart = input.getSpecialKeyState(SpecialKeyCode::RETURN);
    // set cursor position
    input.getMousePosition(GAME_STATE->cursorPosition.x, GAME_STATE->cursorPosition.y);
}

extern "C"
__declspec(dllexport)
void gameUpdate(f64& deltaTime)
{
    // check if player pressed START game
    if (GAME_STATE->actionState.startGame == KeyState::HOLD || GAME_STATE->actionState.startGame == KeyState::PRESS)
    {
        GAME_STATE->startGame();
    }

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
    if (GAME_STATE->actionState.restart == KeyState::HOLD || GAME_STATE->actionState.restart == KeyState::PRESS)
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
    RENDERER->beginBatch(cam.x, cam.y, cam.z);
    GAME_STATE->map.render();
    GAME_STATE->renderSprites();
    RENDERER->endBatch();
}
