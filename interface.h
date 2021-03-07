#pragma once

#include "persistance.h"
#include "game.h"

void render(const struct Piece *board, int *whiteRecord, int *blackRecord, bool isWhiteTurns);
void startMenu();
bool saveFilesMenu(const char **saveFiles, char *filename);
void goodbye();