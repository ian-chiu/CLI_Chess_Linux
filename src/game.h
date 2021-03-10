#pragma once

#include <stdbool.h>

#define BOARD_SIZE 8
#define INPUT_BUFFER_SIZE 256

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
    char from[3];
    char to[3];
    bool invalid;
    bool quit;
    bool restart;
    bool promote;
    bool castle;
    bool save;
    bool load;
} InputProps;

InputProps getUserInput(bool isWhiteTurns);

void init(Piece *board, int *whiteRecord, int *blackRecord, bool *isWhiteTurns);
bool move(InputProps input, Piece *board, bool isWhiteTurns, int *whiteRecord, int *blackRecord);

void render(const Piece *board, int *whiteRecord, int *blackRecord, bool isWhiteTurns);
void startMenu();
bool saveFilesMenu(const char **saveFiles, char *filename);
void goodbye();

void process_quit(bool *finish);
void process_restart(Piece *board, int *whiteRecord, int *blackRecord, bool *isWhiteTurns);
void process_load(Piece *board, int *whiteRecord, int *blackRecord, bool *isWhiteTurns);
void prompt_invalid();
bool hasWinner(int *whiteRecord, int *blackRecord);
void process_win(Piece *board, int *whiteRecord, int *blackRecord, bool *isWhiteTurns, bool *finish);