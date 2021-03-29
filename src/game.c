#include <assert.h>
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
    printf("<pos> promote\n");
    printf("\tpromote a pawn to a queen at <pos>.\n\n");
    printf("undo\n");
    printf("\tget back to the previous action.\n\n");
    printf("redo\n");
    printf("\trecover the latest undo.\n\n");
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

// change a position string to the index in the 1D array board
int posStrToIndex(const char *posStr)
{
    return (BOARD_SIZE - (posStr[1] - '0')) * BOARD_SIZE + (posStr[0] - 'a');
}

// change a position string to a struct Position
Position posStrToPos(const char *posStr)
{
    Position pos = {posStr[0] - 'a', BOARD_SIZE - (posStr[1] - '0')};
    return pos;
}

void init(GameState *gameState, History *history)
{
    gameState->finish = false;
    gameState->isWhiteTurns = true;

    gameState->whiteRecord[None] = 0;
    gameState->whiteRecord[Pawn] = 8;
    gameState->whiteRecord[Knight] = 2;
    gameState->whiteRecord[Bishop] = 2;
    gameState->whiteRecord[Rook] = 2;
    gameState->whiteRecord[Queen] = 1;
    gameState->whiteRecord[King] = 1;

    gameState->blackRecord[None] = 0;
    gameState->blackRecord[Pawn] = 8;
    gameState->blackRecord[Knight] = 2;
    gameState->blackRecord[Bishop] = 2;
    gameState->blackRecord[Rook] = 2;
    gameState->blackRecord[Queen] = 1;
    gameState->blackRecord[King] = 1;

    Piece *board = gameState->board;

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

    History__write(history, gameState);
}

bool move(const InputProps *input, GameState *gameState, History *history)
{
    if (input->castle)
    {
        printf("ERROR: Have not supported castle yet!\n");
        return false;
    }

    // use another variable to store game props items, so we do not have to type 'game->' every time
    Piece *board = gameState->board;
    int *whiteRecord = gameState->whiteRecord;
    int *blackRecord = gameState->blackRecord;
    bool isWhiteTurns = gameState->isWhiteTurns;

    bool success = false;

    // use another variable to store input->from, so we do not have to type 'input->from' every time
    char from[3];
    strcpy(from, input->from);
    // for simplifying calculation, we create a board index and position of 'from'
    int fromIndex = posStrToIndex(from);
    Position fromPos = posStrToPos(from);

    char to[3];
    strcpy(to, input->to);
    int toIndex = posStrToIndex(to);
    Position toPos = posStrToPos(to);

    if (board[fromIndex].type == None) // check if the player moves any piece
    {
        printf("ERROR: There is no any piece on %s!\n", from);
        promptInvalid();
        return false;
    }
    else if (board[fromIndex].isWhite != isWhiteTurns) // check if the player move the component's piece
    {
        printf("ERROR: Cannot move your component's piece!\n");
        promptInvalid();
        return false;
    }
    else if (input->promote) // check if the player promotes a pawn
    {
        if (board[fromIndex].type != Pawn) 
        {
            printf("ERROR: Cannot promote the piece which is not a Pawn!\n");
            promptInvalid();
            return false;
        }
        else
        {
            // if the player promote a white pawn and its position is at row 8, then promote the pawn to a queen
            if (isWhiteTurns && from[1] == '8')
            {
                History__write(history, gameState);
                whiteRecord[Pawn] -= 1;
                whiteRecord[Queen] += 1;
                board[fromIndex].type = Queen;
                // change turns
                gameState->isWhiteTurns = !gameState->isWhiteTurns;
                return true;
            }
            // if the player promote a black pawn and its position is at row 1, then promote the pawn to a queen
            else if (!isWhiteTurns && from[1] == '1')
            {
                History__write(history, gameState);
                blackRecord[Pawn] -= 1;
                blackRecord[Queen] += 1;
                board[fromIndex].type = Queen;
                // change turns
                gameState->isWhiteTurns = !gameState->isWhiteTurns;
                return true;
            }
            else
            {
                printf("ERROR: Cannot promote the Pawn at position %s.\n", from);
                promptInvalid();
                return false;
            }
        }
    }

    // check if there is a player's piece on the 'to' position
    if (board[toIndex].type != None && board[fromIndex].isWhite == board[toIndex].isWhite)
    {
        printf("ERROR: Cannot move to %s!\n", to);
        success =  false;
    }
    else if (from == to)
    {
        printf("ERROR: You did not move any pieces!");
        success = false;
    }
    else
    {
        // check move rules of different type of chess piece
        switch (board[fromIndex].type)
        {
            case None:
                printf("ERROR: There is no any piece at %s\n", from);
                break;
            case Pawn:
            {
                if (board[toIndex].type == None) // Move to empty space
                {
                    if (fromPos.x == toPos.x) // if from and pos is on the same column
                    {
                        if (isWhiteTurns) // if is white player
                        {
                            // if the player move forward one step or move forward two step from row 2 (from white perspective)
                            if (toPos.y == fromPos.y - 1 || (from[1] == '2' && toPos.y == fromPos.y - 2))
                                success = true;
                        }
                        else
                        {
                            // if the player move forward one step or move forward two step from row 2 (from black perspective)
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
                else // Attack enemy (There is an enemy piece on the 'to' position)
                {
                    // check if the horizontal offset is 1 between from and to
                    if (abs(fromPos.x - toPos.x) == 1)
                    {
                        // check if the player moves forward 1 step from the player's perspective
                        // if true then we can move the piece, set 'success' to true
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
                // if player move horizontally
                if (fromPos.x == toPos.x)
                {
                    success = true; // set default success to true
                    int commonX = fromPos.x;
                    // see if there is any piece between from and to (not including from and to)
                    // if there is, then we cannot move the piece, set 'success' to false
                    for (int i = (int)fmin(fromPos.y, toPos.y) + 1; i < (int)fmax(fromPos.y, toPos.y); i++)
                        if (board[i * BOARD_SIZE + commonX].type != None)
                            success = false;
                }
                // if player move vertically
                if (fromPos.y == toPos.y)
                {
                    success = true; // set default success to true
                    int commonY = fromPos.y;
                    // see if there is any piece between from and to (not including from and to)
                    // if there is, then we cannot move the piece, set 'success' to false
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
                // check if the horizontal and vertical offset between from and to are equal
                if (abs(offsetX) == abs(offsetY))
                {
                    success = true; // set default success to true
                    int minX = (int)fmin(fromPos.x, toPos.x);
                    int minY = (int)fmin(fromPos.y, toPos.y);
                    int maxX = (int)fmax(fromPos.x, toPos.x);
                    int maxY = (int)fmax(fromPos.y, toPos.y);
                    int x = minX + 1;
                    // we have to deal with two different diagnoals situation
                    // if the player move on the up right diagnoal(/), then y = minY + 1
                    // if the player move on the up left diagnoal(\), then y = maxY - 1
                    int y = (offsetX * offsetY > 0) ? minY + 1 : maxY - 1;
                    // check if there are any piece between from and to diagnoal (not including from and to)
                    while (x < maxX && y < maxY)
                    {
                        if (board[y * BOARD_SIZE + x].type != None)
                        {
                            success = false;
                            break;
                        }
                        x++;
                        // we have to deal with two different diagnoals situation
                        // if the player move on the up right diagnoal(/), y++
                        // if the player move on the up left diagnoal(\), y--
                        (offsetX * offsetY > 0) ? y++ : y--;
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
                // if the horizontal offset and the vertical offset are less than 1, then we can move the piece
                if (offsetX <= 1 && offsetY <= 1)
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

    // if the move is valid, we move the piece
    if (success)
    {
        // check if there is any enemy piece at 'to'
        // if is, minus the count of piece on the record by one
        if (board[toIndex].type != None)
        {
            if (isWhiteTurns)
                blackRecord[board[toIndex].type] -= 1;
            else
                whiteRecord[board[toIndex].type] -= 1;
        }
        // move the piece from 'from' to 'to'
        board[toIndex] = board[fromIndex];
        // set the chess type at the 'from' position on the board to 'None'
        board[fromIndex].type = None;
        // change turns
        gameState->isWhiteTurns = !gameState->isWhiteTurns;

        History__write(history, gameState);
    }
    else
    {
        promptInvalid();
    }

    return success;
}

void render(const GameState *gameState)
{
    system("clear");
    printf("Black left: px%d nx%d bx%d rx%d qx%d kx%d\n",
           gameState->blackRecord[Pawn],
           gameState->blackRecord[Knight],
           gameState->blackRecord[Bishop],
           gameState->blackRecord[Rook],
           gameState->blackRecord[Queen],
           gameState->blackRecord[King]);
    printf("White left: Px%d Nx%d Bx%d Rx%d Qx%d Kx%d\n",
           gameState->whiteRecord[Pawn],
           gameState->whiteRecord[Knight],
           gameState->whiteRecord[Bishop],
           gameState->whiteRecord[Rook],
           gameState->whiteRecord[Queen],
           gameState->whiteRecord[King]);
    printf("\t  _________________________________\n");

    // we render the board according to the player's perspective
    // if the player is white, we render the '8' row first from top
    // if the player is black, we render the '1' row first from top
    // ...etc
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        gameState->isWhiteTurns ? printf("\t%d |", (9 - (y + 1))) : printf("\t%d |", y + 1);
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            int index = gameState->isWhiteTurns ? y * BOARD_SIZE + x : (7 - y) * BOARD_SIZE + (7 - x);
            switch (gameState->board[index].type)
            {
            case Pawn:
                gameState->board[index].isWhite ? printf(" P ") : printf(" p ");
                break;
            case Knight:
                gameState->board[index].isWhite ? printf(" N ") : printf(" n ");
                break;
            case Bishop:
                gameState->board[index].isWhite ? printf(" B ") : printf(" b ");
                break;
            case Rook:
                gameState->board[index].isWhite ? printf(" R ") : printf(" r ");
                break;
            case Queen:
                gameState->board[index].isWhite ? printf(" Q ") : printf(" q ");
                break;
            case King:
                gameState->board[index].isWhite ? printf(" K ") : printf(" k ");
                break;
            default:
                if (y % 2)
                    index % 2 ? printf("___") : printf("###");
                else
                    index % 2 ? printf("###") : printf("___");
                break;
            }
            printf("|");
        }
        printf("\n");
    }
    gameState->isWhiteTurns ? printf("\t    a   b   c   d   e   f   g   h\n\n") : printf("\t    h   g   f   e   d   c   b   a\n\n");
    gameState->isWhiteTurns ? printf("White turns: \n") : printf("Black turns: \n");
    // if ((fseek(stdin, 0, SEEK_END), ftell(stdin)) > 0)
    // {
    //     char *inputBuffer = NULL;
    //     fgets(stdin, "%s", inputBuffer);
    //     fprintf(stdout, "%s", inputBuffer);
    // }
}

void displayStartMenu()
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
    printf("\t(______)(_______)(_______)(________)(________)(_________)created by jgs    \n\n");
    printAcceptableInputs();
    printf("Press enter to continue...\n");
    getchar();
}

bool displaySaveFilesMenu(const char **saveFiles, char *filename)
{
    system("clear");
    printf("-------------------LOAD GAME-------------------\n\n");
    int nFiles = 0;
    // print all the save files and their corresponding indicies
    for (int i = 0; ; i++) 
    {
        if (!saveFiles[i])
            break;
        printf("%d. %s\n", i+1, saveFiles[i]);
        nFiles++;
    }
    
    // let players choose the save file by the index
    printf("\nPlease input the number of the file: ");
    unsigned int num = 0;
    // if the input is valid 
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

void displayGoodbye()
{
    system("clear");
    printf("----------------------Thanks for playing the game-----------------------\n");
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

void displayHelp()
{
    system("clear");
    printAcceptableInputs();
    pressEnterToContinue();
}

void processUndo(GameState *gameState, History *history)
{
    if (!History__isOldest(history))
    {
        GameState__destroy(gameState);
        gameState = History__undo(history);
    }
}

void processRedo(GameState *gameState, History *history)
{
    if (!History__isNewest(history))
    {
        GameState__destroy(gameState);
        gameState = History__redo(history);
    }
}

void processQuit(GameState *gameState)
{
    printf("--------------------------------------------------------\n");
    printf("Are you sure you want to quit the game?\n");
    printf("(Input 'y' to quit or press 'enter' to cancel...)\n");
    CLEAR_INPUT_BUFFER();
    char result = getchar();
    if (result == 'y' || result == 'Y')
        gameState->finish = true;
}

void processRestart(GameState *gameState, History *history)
{
    printf("--------------------------------------------------------\n");
    printf("Are you sure you want to restart the game?\n");
    printf("(Input 'y' to restart or press 'enter' to cancel...)\n");
    CLEAR_INPUT_BUFFER();
    char result = getchar();
    if (result == 'y' || result == 'Y') {
        GameState__destroy(gameState);
        History__destroy(history);
        gameState = GameState__construct();
        history = History__construct();
        init(gameState, history);
    }
}

void processLoad(GameState *gameState, History *history)
{
    const char *saveFiles[MAX_SAVE_FILES] = { NULL };
    // if we get all the save files from the save directory
    if (getSaveFiles(saveFiles)) 
    {
        char fileName[BUFFER_SIZE] = { '\0' };
        // if we get the file name from the save files menu, then we laod the game from the file
        if (displaySaveFilesMenu(saveFiles, fileName)) 
        {
            loadGame(fileName, gameState);
            History__destroy(history);
            history = History__construct();
            History__write(history, gameState);
        }
    }
    else 
    {
        printf("ERROR: Cannot open 'save' directory!\n");
        pressEnterToContinue();
    }
}

void promptInvalid()
{
    printf("Invalid Input! Please try again.\n");
    pressEnterToContinue();
}

bool hasWinner(GameState *gameState)
{
    // if any player's king is gone, then the game has a winner
    return gameState->whiteRecord[King] <= 0 || gameState->blackRecord[King] <= 0;
}

void processWin(GameState *gameState, History *history)
{
    if (gameState->whiteRecord[King] <= 0)
        printf("CONGRATULATION, BLACK WINS\n");
    if (gameState->blackRecord[King] <= 0)
        printf("CONGRATULATION, WHITE WINS\n");
    printf("Do you want to play again? (Press 'y' to play again or 'enter' to quit...)\n");
    CLEAR_INPUT_BUFFER();
    char result = getchar();
    if (result == 'y' || result == 'Y') 
    {
        GameState__destroy(gameState);
        History__destroy(history);
        gameState = GameState__construct();
        history = History__construct();
        init(gameState, history);
    }
    else 
        gameState->finish = true;
}