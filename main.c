// FIXME:   en passant can only apply immediately after move 2 square from start position 
//          maybe add a history system 
// TODO: castle the king
// TODO: prev move message
#include <stdio.h>
#include "game.h"
#include "input.h"
#include "interface.h"
#include "persistance.h"

int main()
{
    struct Piece board[BOARD_SIZE * BOARD_SIZE];
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
        if (whiteRecord[King] <= 0 || blackRecord[King] <= 0)
        {
            if (whiteRecord[King] <= 0)
                printf("CONGRATULATION, BLACK WINS\n");
            if (blackRecord[King] <= 0)
                printf("CONGRATULATION, WHITE WINS\n");
            printf("Do you want to play again? (Press 'y' to play again or 'enter' to continue...)\n");
            while ((getchar()) != '\n') // clear the input buffer
                ; // null statement
            char result = getchar();
            if (result == 'y' || result == 'Y')
            {
                init(board, whiteRecord, blackRecord, &isWhiteTurns);
                continue;
            }
            else
                break;
        }

        struct InputProps input = getUserInput(isWhiteTurns);

        // check if the player wants to quit or restart the game;
        if (input.quit)
        {
            printf("Are you sure you want to quit the game? (Press 'y' to quit or 'enter' to cancel...)\n");
            while ((getchar()) != '\n')
                ;
            char result = getchar();
            if (result == 'y' || result == 'Y')
                break;
            else
                continue;
        }
        else if (input.restart)
        {
            printf("Are you sure you want to restart the game? (Press 'y' to restart or 'enter' to cancel...)\n");
            while ((getchar()) != '\n') // clear the input buffer
                ; // null statement
            char result = getchar();
            if (result == 'y' || result == 'Y')
                init(board, whiteRecord, blackRecord, &isWhiteTurns);
            continue;
        }
        else if (input.save)
        {
            saveGame(board, whiteRecord, blackRecord, isWhiteTurns);
            continue;
        }
        else if (input.load)
        {
            FileNameArray saveFiles;
            saveFiles.length = 0;
            getSaveFiles(&saveFiles);
            if (saveFiles.length) {
                char fileName[INPUT_BUFFER_SIZE];
                saveFilesMenu(&saveFiles, fileName);
                loadGame(fileName, board, whiteRecord, blackRecord, &isWhiteTurns);
            }
            continue;
        }

        // check if the input or move is valid or not
        if (!input.invalid && move(input, board, isWhiteTurns, whiteRecord, blackRecord))
        {
            isWhiteTurns = !isWhiteTurns;
        }
        else
        {
            printf("Invalid Input! Please try again. Press 'enter' to continue...\n");
            while ((getchar()) != '\n') // clear the input buffer
                ; // null statement
            getchar(); // press any key to continue
        }
    }

    goodbye();
    return 0;
}