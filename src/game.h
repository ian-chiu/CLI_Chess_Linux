#pragma once

#include "input.h"
#include "GameState.h"
#include "History.h"

#include "EventState.h"
#include <ncurses.h>

int posStrToIndex(const char *pos);
Position posStrToPos(const char *posStr);

void init(GameState *gameState, History *history);
bool movePiece(WINDOW* inputWindow, const InputProps *input, GameState *gameState, History *history);
void render(const GameState *gameState, EventState *eventState);

void displayStartMenu();
bool displaySaveFilesMenu(const char **saveFiles, char *filename);
void displayGoodbye();
void displayHelp();
void promptInvalid();

void processUndo(GameState *gameState, History *history);
void processRedo(GameState *gameState, History *history);

void processQuit(WINDOW* inputWindow, GameState *state);
void processRestart(WINDOW* inputWindow, GameState *state, History *history);

void processLoad(GameState *state, History *history);

bool hasWinner(GameState *state);
void processWin(WINDOW* inputWindow, GameState *state, History *history);