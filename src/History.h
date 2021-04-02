#pragma once

#include "GameState.h"
#define MAXHISTORY 10000

typedef struct
{
    GameState* storage[MAXHISTORY];
    int currentIdx;
    int size;
} History;

History* History__construct();
void History__destroy(History *self);
bool History__empty(History *self);
bool History__overflow(History *self);
bool History__isNewest(History *self);
bool History__isOldest(History *self);
void History__write(History *self, const GameState *gameState);
GameState* History__undo(History *self);
GameState* History__redo(History *self);
void History__clear(History *self);
// const GameState* History__pop(History *self);