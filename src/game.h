#pragma once

#include "input.h"
#include "GameState.h"
#include "History.h"

#include "EventState.h"
#include <ncurses.h>

int posStrToIndex(const char *pos);
Position posStrToPos(const char *posStr);

void init(GameState *gameState, History *history);
bool movePiece(WINDOW *gameWindow, const InputProps *input, GameState *gameState, History *history);
void render(WINDOW *gameWindow, const GameState *gameState, EventState *eventState);

void displayStartMenu();
bool displaySaveFilesMenu(const char **saveFiles, char *filename);
void displayGoodbye();
void displayHelp();
void promptInvalid(WINDOW *gameWindow);

void processUndo(GameState *gameState, History *history);
void processRedo(GameState *gameState, History *history);

void processQuit(WINDOW *gameWindow, GameState *state);
void processRestart(WINDOW *gameWindow, GameState *state, History *history);

void processLoad(GameState *state, History *history);

bool hasWinner(GameState *state);
void processWin(WINDOW *gameWindow, GameState *state, History *history);