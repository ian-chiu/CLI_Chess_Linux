#pragma once

#include "game.h"

#define MAX_SAVE_FILES 50

typedef 
struct FileNameArray
{
    char *data[MAX_SAVE_FILES];
    size_t length;
} FileNameArray;


void saveGame(const struct Piece *board, const int *whiteRecord, const int *blackRecord, bool isWhiteTurns);

bool getSaveFiles(FileNameArray *saveFiles);

void loadGame(const char *fileName, struct Piece *board, int *whiteRecord, int *blackRecord, bool *isWhiteTurns);