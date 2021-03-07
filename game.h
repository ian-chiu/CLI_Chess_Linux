#pragma once

#include "input.h"
#define BOARD_SIZE 8

enum ChessType
{
    None,
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
    NumberOfChessType
};

typedef struct 
{
    enum ChessType type;
    bool isWhite;
} Piece;

typedef struct 
{
    int x;
    int y;
} Position;

void init(Piece *board, int *whiteRecord, int *blackRecord, bool *isWhiteTurns);

bool move(InputProps input, Piece *board, bool isWhiteTurns, int *whiteRecord, int *blackRecord);

int posStrToIndex(const char *pos);

Position posStrToPos(const char *posStr);