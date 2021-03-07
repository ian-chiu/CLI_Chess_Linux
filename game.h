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

struct Piece
{
    enum ChessType type;
    bool isWhite;
};

struct Position
{
    int x;
    int y;
};

void init(struct Piece *board, int *whiteRecord, int *blackRecord, bool *isWhiteTurns);

bool move(struct InputProps input, struct Piece *board, bool isWhiteTurns, int *whiteRecord, int *blackRecord);

int posStrToIndex(const char *pos);

struct Position posStrToPos(const char *posStr);