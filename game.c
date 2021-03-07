#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

inline
int posStrToIndex(const char *pos)
{
    return abs(pos[1] - 48 - 8) * BOARD_SIZE + (pos[0] - 97);
}

Position posStrToPos(const char *posStr)
{
    Position pos = {posStr[0] - 97, abs(posStr[1] - 48 - 8)};
    return pos;
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
        };
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