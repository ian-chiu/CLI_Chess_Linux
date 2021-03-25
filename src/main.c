// FIXME:   en passant can only apply 'immediately after' move 2 square from start position 
//          maybe add a history system 
// TODO:    castle the king
#include <stdio.h>
#include "GameState.h"
#include "History.h"
#include "game.h"
#include "fileManagement.h"

int main()
{
    GameState* gameState = GameState__construct();
    History* history = History__construct();

    init(gameState, history);
    displayStartMenu();

    while (!gameState->finish)
    {
        render(gameState);

        if (hasWinner(gameState))
        {
            processWin(gameState, history);
        }
        else
        {
            InputProps input = getUserInput(gameState->isWhiteTurns);

            if (input.quit)
            {
                processQuit(gameState);
            }
            else if (input.restart)
            {
                processRestart(gameState, history);
            }
            else if (input.save)
            {
                saveGame(gameState);
            }
            else if (input.load)
            {
                processLoad(gameState, history);
            }
            else if (input.undo)
            {
                processUndo(gameState, history);
            }
            else if (input.redo)
            {
                processRedo(gameState, history);
            }
            else if (input.help)
            {
                displayHelp();
            }
            else if (!input.invalid)
            {
                move(&input, gameState, history);
            }
            else
            {
                promptInvalid();
            }
        }
    }

    displayGoodbye();
    GameState__destroy(gameState);
    History__destroy(history);
    return 0;
}