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
    Piece board[BOARD_SIZE * BOARD_SIZE];
    int whiteRecord[NumberOfChessType];
    int blackRecord[NumberOfChessType];
    bool isWhiteTurns;
    bool finish;
} GameProps;


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

void init(GameProps *game);
bool move(const InputProps *input, GameProps *game);

void render(const GameProps *game);
void startMenu();
bool saveFilesMenu(const char **saveFiles, char *filename);
void goodbye();

void process_quit(bool *finish);
void process_restart(GameProps *game);
void process_load(GameProps *game);
void prompt_invalid();
bool hasWinner(const GameProps *game);
void process_win(GameProps *game);