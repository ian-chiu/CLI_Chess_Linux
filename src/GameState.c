#include "GameState.h"
#include <stdlib.h>

static void GameState__init(GameState *self)
{
    for (int i =0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        self->board[i].isWhite = true;
        self->board[i].type = None;
    }
    for (int i = 0; i < NumberOfChessType; i++)
    {
        self->whiteRecord[i] = 0;
        self->blackRecord[i] = 0;
    }
    self->isWhiteTurns = true;
    self->finish = false;
}

GameState* GameState__construct()
{
    GameState* result = (GameState*)malloc(sizeof(GameState));
    GameState__init(result);
    return result;
}

GameState* GameState__copyConstruct(const GameState *copy)
{
    GameState* result = (GameState*)malloc(sizeof(GameState));
    for (int i =0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        result->board[i].isWhite = copy->board[i].isWhite;
        result->board[i].type = copy->board[i].type;
    }
    for (int i = 0; i < NumberOfChessType; i++)
    {
        result->whiteRecord[i] = copy->whiteRecord[i];
        result->blackRecord[i] = copy->blackRecord[i];
    }
    result->isWhiteTurns = copy->isWhiteTurns;
    result->finish = copy->finish;
    return result;
}

void GameState__destroy(GameState *self)
{
    free(self);
    self = NULL;
}

void GameState__clear(GameState *self)
{
    GameState__init(self);
}