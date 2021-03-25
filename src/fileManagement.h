#pragma once

#define MAX_SAVE_FILES 50
#define SAVE_FOLDER_PATH CWD "/save/"

#include "GameState.h"

void saveGame(const GameState *gameState);

bool getSaveFiles(const char **saveFiles);

void loadGame(const char *filename, GameState *gameState);