#pragma once

#include <ev.h>
#include <ncurses.h>
#include "GameState.h"
#include "input.h"

// #define ROUND_TIME 10.0

typedef struct
{
    double roundTime;
    double countdown;
    bool timesup;
    const GameState *gameStatePtr;
    char inputStr[BUFFER_SIZE];
} EventState;