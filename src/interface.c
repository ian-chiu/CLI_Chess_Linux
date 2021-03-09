#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "game.h"
#include "persistance.h"

void render(const Piece *board, int *whiteRecord, int *blackRecord, bool isWhiteTurns)
{
    system("clear");
    printf("Black left: ♙ x%d ♘ x%d ♗ x%d ♖ x%d ♕ x%d ♔ x%d\n",
           blackRecord[Pawn],
           blackRecord[Knight],
           blackRecord[Bishop],
           blackRecord[Rook],
           blackRecord[Queen],
           blackRecord[King]);
    printf("White left: ♟︎ x%d ♞ x%d ♝ x%d ♜ x%d ♛ x%d ♚ x%d\n",
           whiteRecord[Pawn],
           whiteRecord[Knight],
           whiteRecord[Bishop],
           whiteRecord[Rook],
           whiteRecord[Queen],
           whiteRecord[King]);
    printf("\t  _________________________\n");
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        isWhiteTurns ? printf("\t%d |", (9 - (y + 1))) : printf("\t%d |", y + 1);
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            int index = isWhiteTurns ? y * BOARD_SIZE + x : (7 - y) * BOARD_SIZE + (7 - x);
            switch (board[index].type)
            {
            case Pawn:
                board[index].isWhite ? printf("♟︎ ") : printf("♙ ");
                break;
            case Knight:
                board[index].isWhite ? printf("♞ ") : printf("♘ ");
                break;
            case Bishop:
                board[index].isWhite ? printf("♝ ") : printf("♗ ");
                break;
            case Rook:
                board[index].isWhite ? printf("♜ ") : printf("♖ ");
                break;
            case Queen:
                board[index].isWhite ? printf("♛ ") : printf("♕ ");
                break;
            case King:
                board[index].isWhite ? printf("♚ ") : printf("♔ ");
                break;
            default:
                if (y % 2)
                    index % 2 ? printf("__") : printf("##");
                else
                    index % 2 ? printf("##") : printf("__");
                break;
            }
            printf("|");
        }
        printf("\n");
    }
    isWhiteTurns ? printf("\t   a  b  c  d  e  f  g  h\n\n") : printf("\t   h  g  f  e  d  c  b  a\n\n");
}

bool saveFilesMenu(const char **saveFiles, char *filename)
{
    system("clear");
    int nFiles = 0;
    for (int i = 0; ; i++) 
    {
        if (!saveFiles[i])
            break;
        printf("%d. %s\n", i+1, saveFiles[i]);
        nFiles++;
    }
    
    printf("Please input the number of the file: ");
    unsigned int num = 0;
    if (scanf("%u", &num) && num < nFiles) 
    {
        strcpy(filename, saveFiles[num - 1]);
        return true;
    }
    else 
    {
        return false;
    }
}

void startMenu()
{
    system("clear");
    printf("--------------------------------WELCOME TO CHESS-------------------------------\n");
    printf("CHESS IS A TWO PLAYER BOARD GAME\n");
    printf("Author: 107802516 電機3B 邱士懿\n");
    printf("\t                                                     _:_        \n");
    printf("\t                                                    '-.-'       \n");
    printf("\t                                           ()      __.'.__      \n");
    printf("\t                                        .-:--:-.  |_______|     \n");
    printf("\t                                 ()      \\____/    \\=====/      \n");
    printf("\t                                 /\\      {====}     )___(       \n");
    printf("\t                      (\\=,      //\\\\      )__(     /_____\\   \n");
    printf("\t      __    |'-'-'|  //  .\\    (    )    /____\\     |   |       \n");
    printf("\t     /  \\   |_____| (( \\_  \\    )__(      |  |      |   |       \n");
    printf("\t     \\__/    |===|   ))  `\\_)  /____\\     |  |      |   |       \n");
    printf("\t    /____\\   |   |  (/     \\    |  |      |  |      |   |       \n");
    printf("\t     |  |    |   |   | _.-'|    |  |      |  |      |   |       \n");
    printf("\t     |__|    )___(    )___(    /____\\    /____\\    /_____\\     \n");
    printf("\t    (====)  (=====)  (=====)  (======)  (======)  (=======)     \n");
    printf("\t    }===={  }====={  }====={  }======{  }======{  }======={     \n");
    printf("\tjgs(______)(_______)(_______)(________)(________)(_________)    \n\n");
    printf("---------------------------------OPERATIONS------------------------------------\n");
    printf("1.\tInput 'quit' to quit the game and 'restart' to restart the game.\n\n");
    printf("2.\tPlease input the position of a piece you want to move and the position\n"
           "\twhere you want to move it to.(<from> <to>).\n\n");
    printf("p.s.\tFor Example, for a white pawn at the initial position a2, if we input\n"
           "\t'a2 a4', then we move the pawn from a2 to a4.\n");
    printf("------------------------------------------------------------------------------\n");
    printf("Press 'enter' to start...");
    getchar();
}

void goodbye()
{
    system("clear");
    printf("--------------------------Thanks for playing the game---------------------------\n");
    printf("\t                       _ _                          \n");
    printf("\t                      | | |                         \n");
    printf("\t  __ _  ___   ___   __| | |__  _   _  ___           \n");
    printf("\t / _` |/ _ \\ / _ \\ / _` | '_ \\| | | |/ _ \\      \n");
    printf("\t| (_| | (_) | (_) | (_| | |_) | |_| |  __/          \n");
    printf("\t \\__, |\\___/ \\___/ \\__,_|_.__/ \\__, |\\___|    \n");
    printf("\t  __/ |                         __/ |               \n");
    printf("\t |___/                         |___/                \n\n");
    printf("Press 'enter' to continue...\n");
    while ((getchar()) != '\n') // clear the input buffer
        ;                       // null statement
    getchar();
    system("clear");
}