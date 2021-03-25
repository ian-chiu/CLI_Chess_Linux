#pragma once

#include <stdbool.h>
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