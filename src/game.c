#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <assert.h>
#include <math.h>
#include "game.h"
#include "persistance.h"

static int posStrToIndex(const char *pos)
{
    return abs(pos[1] - 48 - 8) * BOARD_SIZE + (pos[0] - 97);
}

static Position posStrToPos(const char *posStr)
{
    Position pos = {posStr[0] - 97, abs(posStr[1] - 48 - 8)};
    return pos;
}

InputProps getUserInput(bool isWhiteTurns)
{
    InputProps result;
    result.quit = false;
    result.restart = false;
    result.invalid = false;
    result.promote = false;
    result.castle = false;
    result.save = false;
    result.load = false;

    isWhiteTurns ? printf("White turns: ") : printf("Black turns: ");

    // ---------------input 1------------------
    char input1[20];
    scanf("%s", input1);

    // check if input1 is 'quit'
    if (strcmp(input1, "quit") == 0)
    {
        result.quit = true;
        return result;
    }

    // check if input1 is 'restart'
    if (strcmp(input1, "restart") == 0)
    {
        result.restart = true;
        return result;
    }

    // check if input1 is 'save'
    if (strcmp(input1, "save") == 0)
    {
        result.save = true;
        return result;
    }

    // check if input1 is 'load'
    if (strcmp(input1, "load") == 0)
    {
        result.load = true;
        return result;
    }

    regex_t regex;
    assert(regcomp(&regex, "^[a-zA-Z][1-9]$", REG_ICASE | REG_EXTENDED) == 0);

    // check if input 1 meets the regex "^[a-zA-Z][1-9]$"
    int status1 = regexec(&regex, input1, 0, NULL, 0);
    if (status1 != 0)
    {
        result.invalid = true;
        return result;
    }

    // store first input position to result
    result.from[0] = input1[0];
    result.from[1] = input1[1];
    result.from[2] = '\0';

    // ---------------input 2------------------
    char input2[20];
    scanf("%s", input2);

    // check if input2 is 'promote'
    if (strcmp(input2, "promote") == 0)
    {
        result.promote = true;
        return result;
    }

    // check if input2 is 'castle'
    if (strcmp(input2, "castle") == 0)
    {
        result.castle = true;
        return result;
    }

    int status2 = regexec(&regex, input2, 0, NULL, 0);
    result.invalid = status1 != 0 || status2 != 0;
    if (!result.invalid)
    {
        result.to[0] = input2[0];
        result.to[1] = input2[1];
        result.to[2] = '\0';

        if (
            (posStrToIndex(result.from) >= BOARD_SIZE * BOARD_SIZE || posStrToIndex(result.from) < 0) ||
            (posStrToIndex(result.to) >= BOARD_SIZE * BOARD_SIZE || posStrToIndex(result.to) < 0))
        {
            result.invalid = true;
            printf("ERROR: Input out of range!\n");
        }
    }
    regfree(&regex);
    return result;
}

void init(Piece *board, int *whiteRecord, int *blackRecord, bool *isWhiteTurns)
{
    *isWhiteTurns = true;

    whiteRecord[None] = 0;
    whiteRecord[Pawn] = 8;
    whiteRecord[Knight] = 2;
    whiteRecord[Bishop] = 2;
    whiteRecord[Rook] = 2;
    whiteRecord[Queen] = 1;
    whiteRecord[King] = 1;

    blackRecord[None] = 0;
    blackRecord[Pawn] = 8;
    blackRecord[Knight] = 2;
    blackRecord[Bishop] = 2;
    blackRecord[Rook] = 2;
    blackRecord[Queen] = 1;
    blackRecord[King] = 1;

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

bool move(InputProps input, Piece *board, bool isWhiteTurns, int *whiteRecord, int *blackRecord)
{
    if (input.castle)
    {
        printf("ERROR: Have not supported castle yet!\n");
        return false;
    }

    bool success = false;

    char from[3];
    strcpy(from, input.from);
    int fromIndex = posStrToIndex(from);
    Position fromPos = posStrToPos(from);

    if (board[fromIndex].type == None)
    {
        printf("ERROR: There is no any piece on %s!\n", from);
        return false;
    }
    else if (board[fromIndex].isWhite != isWhiteTurns)
    {
        printf("ERROR: Cannot move your component's piece!\n");
        return false;
    }
    else if (input.promote) // check if user promote a pawn
    {
        if (board[fromIndex].type != Pawn)
            printf("ERROR: Cannot promote the piece which is not a Pawn!");
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
            }
            return success;
        }
    }

    char to[3];
    strcpy(to, input.to);
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
                    // en passant
                    int enemyIndex = fromPos.y * BOARD_SIZE + toPos.x;
                    bool hasEnemy = board[enemyIndex].type != None;
                    if (hasEnemy && abs(fromPos.x - toPos.x) == 1)
                    {
                        if (isWhiteTurns)
                        {
                            if (from[1] == '5' && toPos.y == fromPos.y - 1)
                            {
                                success = true;
                                blackRecord[board[enemyIndex].type] -= 1;
                                board[enemyIndex].type = None;
                            }
                        }
                        else
                        {
                            if (from[1] == '4' && toPos.y == fromPos.y + 1)
                            {
                                success = true;
                                whiteRecord[board[enemyIndex].type] -= 1;
                                board[enemyIndex].type = None;
                            }
                        }
                    }
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
                printf("ERROR: No such type of chess piece.");
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
    }

    return success;
}

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

void process_quit(bool *finish)
{
    printf("Are you sure you want to quit the game? (Press 'y' to quit or 'enter' to cancel...)\n");
    while ((getchar()) != '\n')
        ; // null statement
    char result = getchar();
    if (result == 'y' || result == 'Y')
        *finish = true;
}

void process_restart(Piece *board, int *whiteRecord, int *blackRecord, bool *isWhiteTurns)
{
    printf("Are you sure you want to restart the game? (Press 'y' to restart or 'enter' to cancel...)\n");
    while ((getchar()) != '\n') // clear the input buffer
        ; // null statement
    char result = getchar();
    if (result == 'y' || result == 'Y')
        init(board, whiteRecord, blackRecord, isWhiteTurns);
}

void process_load(Piece *board, int *whiteRecord, int *blackRecord, bool *isWhiteTurns)
{
    const char *saveFiles[MAX_SAVE_FILES] = { NULL };
    if (getSaveFiles(saveFiles)) {
        char fileName[INPUT_BUFFER_SIZE] = { "" };
        if (saveFilesMenu(saveFiles, fileName))
            loadGame(fileName, board, whiteRecord, blackRecord, isWhiteTurns);
    }
    else {
        printf("ERROR: Cannot open 'save' directory!\n");
        while ((getchar()) != '\n') // clear the input buffer
            ; // null statement
        getchar(); // press any key to continue
    }
}

void prompt_invalid()
{
    prompt_invalid();
    printf("Invalid Input! Please try again. Press 'enter' to continue...\n");
    while ((getchar()) != '\n') // clear the input buffer
        ; // null statement
    getchar(); // press any key to continue
}

bool hasWinner(int *whiteRecord, int *blackRecord)
{
    return whiteRecord[King] <= 0 || blackRecord[King] <= 0;
}

void process_win(Piece *board, int *whiteRecord, int *blackRecord, bool *isWhiteTurns, bool *finish)
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
        init(board, whiteRecord, blackRecord, isWhiteTurns);
    else 
        *finish = true;
}