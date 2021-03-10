#pragma once

#define MAX_SAVE_FILES 50
#define SAVE_FOLDER_PATH CWD "/save/"

#include "game.h"

void saveGame(const GameProps *game);

bool getSaveFiles(const char **saveFiles);

void loadGame(const char *filename, GameProps *game);