#include "Memento.h"
#include <stdlib.h>

static void Memento__init(Memento *self, const GameState *copy)
{
    for (int i =0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        self->gameState.board[i].isWhite = copy->board[i].isWhite;
        self->gameState.board[i].type = copy->board[i].type;
    }
    for (int i = 0; i < NumberOfChessType; i++)
    {
        self->gameState.whiteRecord[i] = copy->whiteRecord[i];
        self->gameState.blackRecord[i] = copy->blackRecord[i];
    }
    self->gameState.isWhiteTurns = copy->isWhiteTurns;
    self->gameState.finish = copy->finish;
}

Memento* Memento__create(const GameState *gameState)
{
    Memento* result = (Memento*)malloc(sizeof(Memento));
    Memento__init(result, gameState);
    return result;
}

void Memento__destory(Memento *self)
{
    free(self);
}