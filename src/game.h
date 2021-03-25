#pragma once

#include "input.h"
#include "GameState.h"
#include "History.h"

int posStrToIndex(const char *pos);
Position posStrToPos(const char *posStr);

void init(GameState *gameState, History *history);
bool move(const InputProps *input, GameState *gameState, History *history);
void render(const GameState *state);

void displayStartMenu();
bool displaySaveFilesMenu(const char **saveFiles, char *filename);
void displayGoodbye();
void displayHelp();
void promptInvalid();

void processUndo(GameState *gameState, History *history);
void processRedo(GameState *gameState, History *history);

void processQuit(GameState *state);
void processRestart(GameState *state, History *history);

void processLoad(GameState *state, History *history);

bool hasWinner(GameState *state);
void processWin(GameState *state, History *history);