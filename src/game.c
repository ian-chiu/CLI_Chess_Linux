#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "game.h"
#include "fileManagement.h"

#define CLEAR_INPUT_BUFFER() \
    int c; \
    while (!feof(stdin) && (c = getchar()) != '\n' && c != EOF) { }

static void pressEnterToContinue() 
{
    printf("Press enter to continue...\n");
    CLEAR_INPUT_BUFFER();
    char enter = 0;
    while (enter != '\r' && enter != '\n') { enter = getchar(); }
}

static void printAcceptableInputs()
{
    printf("------------------------------------------------------------------------------\n");
    printf("ACCEPTABLE INPUTS\n\n");
    printf("<pos1> <pos2>\n");
    printf("\tmove a piece from <pos1> to <pos2>.\n\n");
    printf("<pos1> promote\n");
    printf("\tpromote a pawn to queen at <pos1>.\n\n");
    printf("save\n");
    printf("\tsave the game to a file.\n\n");
    printf("load\n");
    printf("\tload the game from a save file.\n\n");
    printf("quit\n");
    printf("\tquit the game.\n\n");
    printf("restart\n");
    printf("\trestart the game.\n\n");
    printf("help\n");
    printf("\tsee all acceptable inputs.\n\n");
    printf("The game does not support 'castle the king' and 'en passant'.\n");
    printf("------------------------------------------------------------------------------\n");
}

int posStrToIndex(const char *pos)
{
    return abs(pos[1] - 48 - 8) * BOARD_SIZE + (pos[0] - 97);
}

Position posStrToPos(const char *posStr)
{
    Position pos = {posStr[0] - 97, abs(posStr[1] - 48 - 8)};
    return pos;
}

void init(GameProps *game)
{
    game->finish = false;
    game->isWhiteTurns = true;

    game->whiteRecord[None] = 0;
    game->whiteRecord[Pawn] = 8;
    game->whiteRecord[Knight] = 2;
    game->whiteRecord[Bishop] = 2;
    game->whiteRecord[Rook] = 2;
    game->whiteRecord[Queen] = 1;
    game->whiteRecord[King] = 1;

    game->blackRecord[None] = 0;
    game->blackRecord[Pawn] = 8;
    game->blackRecord[Knight] = 2;
    game->blackRecord[Bishop] = 2;
    game->blackRecord[Rook] = 2;
    game->blackRecord[Queen] = 1;
    game->blackRecord[King] = 1;

    Piece *board = game->board;

    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
    {
        board[i].type = None;
        board[i].isWhite = true;
    }

    // white
    board[posStrToIndex("a2")].isWhite = true;
    board[posStrToIndex("a2")].type = Pawn;
    board[posStrToIndex("b2")].isWhite = true;
    board[posStrToIndex("b2")].type = Pawn;
    board[posStrToIndex("c2")].isWhite = true;
    board[posStrToIndex("c2")].type = Pawn;
    board[posStrToIndex("d2")].isWhite = true;
    board[posStrToIndex("d2")].type = Pawn;
    board[posStrToIndex("e2")].isWhite = true;
    board[posStrToIndex("e2")].type = Pawn;
    board[posStrToIndex("f2")].isWhite = true;
    board[posStrToIndex("f2")].type = Pawn;
    board[posStrToIndex("g2")].isWhite = true;
    board[posStrToIndex("g2")].type = Pawn;
    board[posStrToIndex("h2")].isWhite = true;
    board[posStrToIndex("h2")].type = Pawn;

    board[posStrToIndex("a1")].isWhite = true;
    board[posStrToIndex("a1")].type = Rook;
    board[posStrToIndex("h1")].isWhite = true;
    board[posStrToIndex("h1")].type = Rook;

    board[posStrToIndex("b1")].isWhite = true;
    board[posStrToIndex("b1")].type = Knight;
    board[posStrToIndex("g1")].isWhite = true;
    board[posStrToIndex("g1")].type = Knight;

    board[posStrToIndex("c1")].isWhite = true;
    board[posStrToIndex("c1")].type = Bishop;
    board[posStrToIndex("f1")].isWhite = true;
    board[posStrToIndex("f1")].type = Bishop;

    board[posStrToIndex("d1")].isWhite = true;
    board[posStrToIndex("d1")].type = Queen;
    board[posStrToIndex("e1")].isWhite = true;
    board[posStrToIndex("e1")].type = King;

    // black
    board[posStrToIndex("a7")].isWhite = false;
    board[posStrToIndex("a7")].type = Pawn;
    board[posStrToIndex("b7")].isWhite = false;
    board[posStrToIndex("b7")].type = Pawn;
    board[posStrToIndex("c7")].isWhite = false;
    board[posStrToIndex("c7")].type = Pawn;
    board[posStrToIndex("d7")].isWhite = false;
    board[posStrToIndex("d7")].type = Pawn;
    board[posStrToIndex("e7")].isWhite = false;
    board[posStrToIndex("e7")].type = Pawn;
    board[posStrToIndex("f7")].isWhite = false;
    board[posStrToIndex("f7")].type = Pawn;
    board[posStrToIndex("g7")].isWhite = false;
    board[posStrToIndex("g7")].type = Pawn;
    board[posStrToIndex("h7")].isWhite = false;
    board[posStrToIndex("h7")].type = Pawn;

    board[posStrToIndex("a8")].isWhite = false;
    board[posStrToIndex("a8")].type = Rook;
    board[posStrToIndex("h8")].isWhite = false;
    board[posStrToIndex("h8")].type = Rook;

    board[posStrToIndex("b8")].isWhite = false;
    board[posStrToIndex("b8")].type = Knight;
    board[posStrToIndex("g8")].isWhite = false;
    board[posStrToIndex("g8")].type = Knight;

    board[posStrToIndex("c8")].isWhite = false;
    board[posStrToIndex("c8")].type = Bishop;
    board[posStrToIndex("f8")].isWhite = false;
    board[posStrToIndex("f8")].type = Bishop;

    board[posStrToIndex("d8")].isWhite = false;
    board[posStrToIndex("d8")].type = Queen;
    board[posStrToIndex("e8")].isWhite = false;
    board[posStrToIndex("e8")].type = King;
}

bool move(const InputProps *input, GameProps *game)
{
    if (input->castle)
    {
        printf("ERROR: Have not supported castle yet!\n");
        return false;
    }

    Piece *board = game->board;
    int *whiteRecord = game->whiteRecord;
    int *blackRecord = game->blackRecord;
    bool isWhiteTurns = game->isWhiteTurns;

    bool success = false;

    char from[3];
    strcpy(from, input->from);
    int fromIndex = posStrToIndex(from);
    Position fromPos = posStrToPos(from);

    if (board[fromIndex].type == None)
    {
        printf("ERROR: There is no any piece on %s!\n", from);
        prompt_invalid();
        return false;
    }
    else if (board[fromIndex].isWhite != isWhiteTurns)
    {
        printf("ERROR: Cannot move your component's piece!\n");
        prompt_invalid();
        return false;
    }
    else if (input->promote) // check if user promote a pawn
    {
        if (board[fromIndex].type != Pawn) 
        {
            printf("ERROR: Cannot promote the piece which is not a Pawn!");
            prompt_invalid();
        }
        else
        {
            if (isWhiteTurns && from[1] == '8')
            {
                whiteRecord[Pawn] -= 1;
                whiteRecord[Queen] += 1;
                board[fromIndex].type = Queen;
                success = true;
            }
            else if (!isWhiteTurns && from[1] == '1')
            {
                blackRecord[Pawn] -= 1;
                blackRecord[Queen] += 1;
                board[fromIndex].type = Queen;
                success = true;
            }
            else
            {
                printf("ERROR: Cannot promote the Pawn at position %s", from);
                prompt_invalid();
            }
            return success;
        }
    }

    char to[3];
    strcpy(to, input->to);
    int toIndex = posStrToIndex(to);
    Position toPos = posStrToPos(to);

    if (board[toIndex].type != None && board[fromIndex].isWhite == board[toIndex].isWhite)
    {
        printf("ERROR: Cannot move to %s!\n", to);
        return false;
    }
    else if (from == to)
    {
        printf("ERROR: You did not move any pieces!");
        return false;
    }
    else
    {
        switch (board[fromIndex].type)
        {
            case None:
                printf("ERROR: There is no any piece at %s\n", from);
                break;
            case Pawn:
            {
                if (board[toIndex].type == None) // Move to empty space
                {
                    if (fromPos.x == toPos.x)
                    {
                        if (isWhiteTurns)
                        {
                            if (toPos.y == fromPos.y - 1 || (from[1] == '2' && toPos.y == fromPos.y - 2))
                                success = true;
                        }
                        else
                        {
                            if (toPos.y == fromPos.y + 1 || (from[1] == '7' && toPos.y == fromPos.y + 2))
                                success = true;
                        }
                    }
                    // --------------------TODO: en passant--------------------
                    // int enemyIndex = fromPos.y * BOARD_SIZE + toPos.x;
                    // bool hasEnemy = board[enemyIndex].type != None;
                    // if (hasEnemy && abs(fromPos.x - toPos.x) == 1)
                    // {
                    //     if (isWhiteTurns)
                    //     {
                    //         if (from[1] == '5' && toPos.y == fromPos.y - 1)
                    //         {
                    //             success = true;
                    //             blackRecord[board[enemyIndex].type] -= 1;
                    //             board[enemyIndex].type = None;
                    //         }
                    //     }
                    //     else
                    //     {
                    //         if (from[1] == '4' && toPos.y == fromPos.y + 1)
                    //         {
                    //             success = true;
                    //             whiteRecord[board[enemyIndex].type] -= 1;
                    //             board[enemyIndex].type = None;
                    //         }
                    //     }
                    // }
                }
                else // Attack enemy
                {
                    if (abs(fromPos.x - toPos.x) == 1)
                    {
                        if (isWhiteTurns)
                        {
                            if (toPos.y == fromPos.y - 1)
                                success = true;
                        }
                        else
                        {
                            if (toPos.y == fromPos.y + 1)
                                success = true;
                        }
                    }
                }
                if (success == false)
                    printf("ERROR: Invalid move of Pawn!\n");
                break;
            }
            case Rook:
            {
                if (fromPos.x == toPos.x)
                {
                    success = true; // set default success to true
                    int commonX = fromPos.x;
                    for (int i = (int)fmin(fromPos.y, toPos.y) + 1; i < (int)fmax(fromPos.y, toPos.y); i++)
                        if (board[i * BOARD_SIZE + commonX].type != None)
                            success = false;
                }
                if (fromPos.y == toPos.y)
                {
                    success = true; // set default success to true
                    int commonY = fromPos.y;
                    for (int i = (int)fmin(fromPos.x, toPos.x) + 1; i < (int)fmax(fromPos.x, toPos.x); i++)
                        if (board[commonY * BOARD_SIZE + i].type != None)
                            success = false;
                }
                if (success == false)
                    printf("ERROR: Invalid move of Rook!\n");
                break;
            }
            case Bishop:
            {
                int offsetY = fromPos.y - toPos.y;
                int offsetX = fromPos.x - toPos.x;
                if (abs(offsetX) == abs(offsetY))
                {
                    success = true; // set default success to true
                    int minX = (int)fmin(fromPos.x, toPos.x);
                    int minY = (int)fmin(fromPos.y, toPos.y);
                    int maxX = (int)fmax(fromPos.x, toPos.x);
                    int maxY = (int)fmax(fromPos.y, toPos.y);
                    int x = minX + 1;
                    int y = offsetX * offsetY > 0 ? minY + 1 : maxY - 1;
                    while (x < maxX && y < maxY)
                    {
                        if (board[y * BOARD_SIZE + x].type != None)
                        {
                            success = false;
                            break;
                        }
                        x++;
                        offsetX * offsetY > 0 ? y++ : y--;
                    
                    }
                }
                if (success == false)
                    printf("ERROR: Invalid move of Bishop!\n");
                break;
            }
            case Knight:
            {
                int offsetY = abs(fromPos.y - toPos.y);
                int offsetX = abs(fromPos.x - toPos.x);
                if ((offsetX == 2 && offsetY == 1) || (offsetY == 2 && offsetX == 1))
                    success = true;
                if (success == false)
                    printf("ERROR: Invalid move of Knight!\n");
                break;
            }
            case Queen:
            {
                // check if it applys to rook move
                if (fromPos.x == toPos.x)
                {
                    success = true; // set default success to true
                    int commonX = fromPos.x;
                    for (int i = (int)fmin(fromPos.y, toPos.y) + 1; i < (int)fmax(fromPos.y, toPos.y); i++)
                        if (board[i * BOARD_SIZE + commonX].type != None)
                            success = false;
                }
                if (fromPos.y == toPos.y)
                {
                    success = true; // set default success to true
                    int commonY = fromPos.y;
                    for (int i = (int)fmin(fromPos.x, toPos.x) + 1; i < (int)fmax(fromPos.x, toPos.x); i++)
                        if (board[commonY * BOARD_SIZE + i].type != None)
                            success = false;
                }

                // check if it applys to bishop move
                int offsetY = fromPos.y - toPos.y;
                int offsetX = fromPos.x - toPos.x;
                if (abs(offsetX) == abs(offsetY))
                {
                    success = true; // set default success to true
                    int minX = (int)fmin(fromPos.x, toPos.x);
                    int minY = (int)fmin(fromPos.y, toPos.y);
                    int maxX = (int)fmax(fromPos.x, toPos.x);
                    int maxY = (int)fmax(fromPos.y, toPos.y);
                    int x = minX + 1;
                    int y = offsetX * offsetY > 0 ? minY + 1 : maxY - 1;
                    while (x < maxX && y < maxY)
                    {
                        if (board[y * BOARD_SIZE + x].type != None)
                        {
                            success = false;
                            break;
                        }
                        x++;
                        offsetX * offsetY > 0 ? y++ : y--;
                    
                    }
                }

                if (success == false)
                    printf("ERROR: Invalid move of Queen!\n");
                break;
            }
            case King:
            {
                int offsetY = abs(fromPos.y - toPos.y);
                int offsetX = abs(fromPos.x - toPos.x);
                if (offsetX + offsetY <= 2)
                    success = true;
                if (success == false)
                    printf("ERROR: Invalid move of King!\n");
                break;
            }
            default:
            {
                printf("ERROR: No such type of chess piece.\n");
            }
        }
        
    }

    if (success)
    {
        if (board[toIndex].type != None)
        {
            if (isWhiteTurns)
                blackRecord[board[toIndex].type] -= 1;
            else
                whiteRecord[board[toIndex].type] -= 1;
        }
        board[toIndex] = board[fromIndex];
        board[fromIndex].type = None;
        game->isWhiteTurns = !game->isWhiteTurns;
    }
    else
    {
        prompt_invalid();
    }

    return success;
}

void render(const GameProps *game)
{
    system("clear");
    printf("Black left: ♙ x%d ♘ x%d ♗ x%d ♖ x%d ♕ x%d ♔ x%d\n",
           game->blackRecord[Pawn],
           game->blackRecord[Knight],
           game->blackRecord[Bishop],
           game->blackRecord[Rook],
           game->blackRecord[Queen],
           game->blackRecord[King]);
    printf("White left: ♟︎ x%d ♞ x%d ♝ x%d ♜ x%d ♛ x%d ♚ x%d\n",
           game->whiteRecord[Pawn],
           game->whiteRecord[Knight],
           game->whiteRecord[Bishop],
           game->whiteRecord[Rook],
           game->whiteRecord[Queen],
           game->whiteRecord[King]);
    printf("\t  _________________________\n");
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        game->isWhiteTurns ? printf("\t%d |", (9 - (y + 1))) : printf("\t%d |", y + 1);
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            int index = game->isWhiteTurns ? y * BOARD_SIZE + x : (7 - y) * BOARD_SIZE + (7 - x);
            switch (game->board[index].type)
            {
            case Pawn:
                game->board[index].isWhite ? printf("♟︎ ") : printf("♙ ");
                break;
            case Knight:
                game->board[index].isWhite ? printf("♞ ") : printf("♘ ");
                break;
            case Bishop:
                game->board[index].isWhite ? printf("♝ ") : printf("♗ ");
                break;
            case Rook:
                game->board[index].isWhite ? printf("♜ ") : printf("♖ ");
                break;
            case Queen:
                game->board[index].isWhite ? printf("♛ ") : printf("♕ ");
                break;
            case King:
                game->board[index].isWhite ? printf("♚ ") : printf("♔ ");
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
    game->isWhiteTurns ? printf("\t   a  b  c  d  e  f  g  h\n\n") : printf("\t   h  g  f  e  d  c  b  a\n\n");
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
    if (scanf("%u", &num) && num <= nFiles) 
    {
        strcpy(filename, saveFiles[num - 1]);
        return true;
    }
    else 
    {
        return false;
    }
}

void help()
{
    system("clear");
    printAcceptableInputs();
    pressEnterToContinue();
}

void startMenu()
{
    system("clear");
    printf("--------------------------------WELCOME TO CHESS-------------------------------\n");
    printf("CHESS IS A TWO PLAYER BOARD GAME\n");
    printf("Author: 107802516 電機3B 邱士懿\n");
    printf("\t                                                  _:_                     \n");
    printf("\t                                                 '-.-'                    \n");
    printf("\t                                        ()      __.'.__                   \n");
    printf("\t                                     .-:--:-.  |_______|                  \n");
    printf("\t                              ()      \\____/    \\=====/                 \n");
    printf("\t                              /\\      {====}     )___(                   \n");
    printf("\t                   (\\=,      //\\\\      )__(     /_____\\               \n");
    printf("\t   __    |'-'-'|  //  .\\    (    )    /____\\     |   |                  \n");
    printf("\t  /  \\   |_____| (( \\_  \\    )__(      |  |      |   |                 \n");
    printf("\t  \\__/    |===|   ))  `\\_)  /____\\     |  |      |   |                 \n");
    printf("\t /____\\   |   |  (/     \\    |  |      |  |      |   |                  \n");
    printf("\t  |  |    |   |   | _.-'|    |  |      |  |      |   |                    \n");
    printf("\t  |__|    )___(    )___(    /____\\    /____\\    /_____\\                \n");
    printf("\t (====)  (=====)  (=====)  (======)  (======)  (=======)                  \n");
    printf("\t }===={  }====={  }====={  }======{  }======{  }======={                  \n");
    printf("\t(______)(_______)(_______)(________)(________)(_________)create by jgs    \n\n");
    printAcceptableInputs();
    printf("Press enter to continue...\n");
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
    pressEnterToContinue();
    system("clear");
}

void process_quit(bool *finish)
{
    printf("--------------------------------------------------------\n");
    printf("Are you sure you want to quit the game?\n");
    printf("(Input 'y' to restart or press 'enter' to cancel...)\n");
    CLEAR_INPUT_BUFFER();
    char result = getchar();
    if (result == 'y' || result == 'Y')
        *finish = true;
}

void process_restart(GameProps *game)
{
    printf("--------------------------------------------------------\n");
    printf("Are you sure you want to restart the game?\n");
    printf("(Input 'y' to restart or press 'enter' to cancel...)\n");
    CLEAR_INPUT_BUFFER();
    char result = getchar();
    if (result == 'y' || result == 'Y')
        init(game);
}

void process_load(GameProps *game)
{
    const char *saveFiles[MAX_SAVE_FILES] = { NULL };
    if (getSaveFiles(saveFiles)) {
        char fileName[BUFFER_SIZE] = { "" };
        if (saveFilesMenu(saveFiles, fileName))
            loadGame(fileName, game);
    }
    else {
        printf("ERROR: Cannot open 'save' directory!\n");
        pressEnterToContinue();
    }
}

void prompt_invalid()
{
    printf("Invalid Input! Please try again.\n");
    pressEnterToContinue();
}

bool hasWinner(const GameProps *game)
{
    return game->whiteRecord[King] <= 0 || game->blackRecord[King] <= 0;
}

void process_win(GameProps *game)
{
    if (game->whiteRecord[King] <= 0)
        printf("CONGRATULATION, BLACK WINS\n");
    if (game->blackRecord[King] <= 0)
        printf("CONGRATULATION, WHITE WINS\n");
    printf("Do you want to play again? (Press 'y' to play again or 'enter' to continue...)\n");
    CLEAR_INPUT_BUFFER();
    char result = getchar();
    if (result == 'y' || result == 'Y')
        init(game);
    else 
        game->finish = true;
}