#pragma once
#include "Defines.h"
#include "Components.h"

enum State 
{
    GAME_STATE_MAIN_MENU,
    GAME_STATE_SETTINGS,
    GAME_STATE_START,
    GAME_STATE_RUNNING,
    GAME_STATE_PAUSED,
    GAME_STATE_LOST,
    GAME_STATE_COUNT
};

struct GameState
{
    State state;
    
    //TODO: maybe split these depending on the state? 
    Entity playerEId;
    EntityRegistry<ComponentList<KAMSKI_COMPONENTS>> entityRegistry;
    glm::vec3 camera;
    ItemSet itemSet;
    ActionStates actionState;
    bool isVroomOn;
};
