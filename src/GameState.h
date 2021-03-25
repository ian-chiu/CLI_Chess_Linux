#pragma once

#include "gameProps.h"

typedef struct 
{
    Piece board[BOARD_SIZE * BOARD_SIZE];
    int whiteRecord[NumberOfChessType];
    int blackRecord[NumberOfChessType];
    bool isWhiteTurns;
    bool finish;
} GameState;

GameState* GameState__construct();
void GameState__destroy(GameState *self);
GameState* GameState__copyConstruct(const GameState *copy);