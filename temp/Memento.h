#pragma once

#include <stdbool.h>
#include "GameProps.h"

typedef struct 
{
    GameState gameState;
} Memento;

Memento* Memento__create(const GameState *gameState);
void Memento__destory(Memento *self);