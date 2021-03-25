#pragma once

#include "input.h"
#include "Memento.h"
#include "History.h"

typedef struct
{
    GameState gameState;
} Game;

int posStrToIndex(const char *pos);
Position posStrToPos(const char *posStr);

Game* Game__create();
void Game__destroy(Game *self);

Memento* Game__createMemento(Game *self);
void Game__restore(Game *self, const Memento* mementoPtr);

bool Game__move(Game *self, const InputProps *input);
void Game__render(const Game *self);
void Game__displayStartMenu();
bool Game__displaySaveFilesMenu(const char **saveFiles, char *filename);
void Game__displayGoodbye();
void Game__displayHelp();
void Game__processQuit(Game *self);
void Game__processRestart(Game *self, History *history);
void Game__processLoad(Game *self, History *history);
void Game__promptInvalid();
bool Game__hasWinner(Game *self);
void Game__processWin(Game *self, History *history);