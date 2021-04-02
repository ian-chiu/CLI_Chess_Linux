#include <stdlib.h>
#include <assert.h>
#include "History.h"

static void History__init(History *self)
{
    for (int i = 0; i < MAXHISTORY; i++)
        self->storage[i] = NULL;
    self->size = 0;
    self->currentIdx = -1;
}

History* History__construct()
{
    History* result = (History*)malloc(sizeof(History));
    History__init(result);
    return result;
}

void History__destroy(History *self)
{
    for (int i = 0; i < self->size; i++) 
    {
        free(self->storage[i]);
        self->storage[i] = NULL;
    }
    free(self);
    self = NULL;
}

bool History__empty(History *self)
{
    return self->size <= 0;
}

bool History__isNewest(History *self)
{
    assert(!History__overflow(self));
    return self->currentIdx == self->size - 1;
}

bool History__isOldest(History *self)
{
    assert(!History__empty(self));
    return self->currentIdx <= 0;
}

bool History__overflow(History *self)
{
    return self->size >= MAXHISTORY;
}

void History__write(History *self, const GameState *gameState)
{
    assert(!History__overflow(self));
    self->storage[++(self->currentIdx)] = GameState__copyConstruct(gameState);
    if (self->currentIdx == self->size)
        self->size = self->currentIdx + 1;
}

GameState* History__undo(History *self)
{
    assert(!History__isOldest(self));
    self->currentIdx -= 1;
    GameState *lastGameState = GameState__copyConstruct(self->storage[self->currentIdx]);
    return lastGameState;
}

GameState* History__redo(History *self)
{
    assert(!History__isNewest(self));
    self->currentIdx += 1;
    GameState *nextGameState = GameState__copyConstruct(self->storage[self->currentIdx]);
    return nextGameState;
}

void History__clear(History *self)
{
    History__init(self);
}

// const GameState* History__pop(History *self)
// {
//     assert(!History__empty(self));
//     const GameState *lastGameState = self->storage[self->size - 1];
//     self->storage[self->size - 1] = NULL;
//     self->size--;
//     return lastGameState;
// }