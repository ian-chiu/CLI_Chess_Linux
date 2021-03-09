#pragma once

#include "game.h"

#define MAX_SAVE_FILES 50

#define SAVE_FOLDER_PATH CWD "/save/"

void saveGame(const Piece *board, const int *whiteRecord, const int *blackRecord, bool isWhiteTurns);

bool getSaveFiles(const char **saveFiles);

void loadGame(const char *fileName, Piece *board, int *whiteRecord, int *blackRecord, bool *isWhiteTurns);