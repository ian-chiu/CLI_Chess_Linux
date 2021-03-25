#pragma once

#include <stdbool.h>
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

typedef struct 
{
    // we use 1D array to store 2D information of the board
    // the formula of using 2D coordinate (x, y) to get the index of the board is:
    //      index = y * BOARD_SIZE + x
    Piece board[BOARD_SIZE * BOARD_SIZE];
    int whiteRecord[NumberOfChessType];
    int blackRecord[NumberOfChessType];
    bool isWhiteTurns;
    bool finish;
} GameProps;

int posStrToIndex(const char *pos);
Position posStrToPos(const char *posStr);

void init(GameProps *game);
bool move(const InputProps *input, GameProps *game);

void render(const GameProps *game);
void startMenu();
bool saveFilesMenu(const char **saveFiles, char *filename);
void goodbye();
void help();

void process_quit(bool *finish);
void process_restart(GameProps *game);
void process_load(GameProps *game);
void prompt_invalid();
bool hasWinner(const GameProps *game);
void process_win(GameProps *game);