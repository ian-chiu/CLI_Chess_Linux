// FIXME:   en passant can only apply immediately after move 2 square from start position 
//          maybe add a history system 
// TODO: castle the king
// TODO: prev move message
#include <stdio.h>
#include "game.h"
#include "persistance.h"

int main()
{
    Piece board[BOARD_SIZE * BOARD_SIZE];
    int whiteRecord[NumberOfChessType];
    int blackRecord[NumberOfChessType];
    bool isWhiteTurns;
    bool finish = false;

    init(board, whiteRecord, blackRecord, &isWhiteTurns);
    startMenu();

    while (!finish)
    {
        render(board, whiteRecord, blackRecord, isWhiteTurns);

        // check if a winner appears
        if (hasWinner(whiteRecord, blackRecord))
        {
            process_win(board, whiteRecord, blackRecord, &isWhiteTurns, &finish);
        }
        else
        {
            InputProps input = getUserInput(isWhiteTurns);

            // check if the player wants to quit or restart the game;
            if (input.quit)
            {
                process_quit(&finish);
            }
            else if (input.restart)
            {
                process_restart(board, whiteRecord, blackRecord, &isWhiteTurns);
            }
            else if (input.save)
            {
                saveGame(board, whiteRecord, blackRecord, isWhiteTurns);
            }
            else if (input.load)
            {
                process_load(board, whiteRecord, blackRecord, &isWhiteTurns);
            }
            else if (!input.invalid && move(input, board, isWhiteTurns, whiteRecord, blackRecord))
            {
                isWhiteTurns = !isWhiteTurns;
            }
            else
            {
                prompt_invalid();
            }
        }
    }

    goodbye();
    return 0;
}