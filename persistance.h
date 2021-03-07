#pragma once

#include "game.h"

#define MAX_SAVE_FILES 50

void saveGame(const struct Piece *board, const int *whiteRecord, const int *blackRecord, bool isWhiteTurns);

bool getSaveFiles(const char **saveFiles);

void loadGame(const char *fileName, struct Piece *board, int *whiteRecord, int *blackRecord, bool *isWhiteTurns);