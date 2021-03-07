#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include "persistance.h"
#include "game.h"
#include "input.h"

bool getSaveFiles(FileNameArray *saveFiles)
{
    struct dirent *entry;
    DIR *dir = opendir("./save");
    if (!dir) {
        printf("ERROR: Cannot open './save' directory!");
        return false;
    }
    int length = 0;
    while ((entry = readdir(dir)) != NULL) 
        if (entry->d_name[0] != '.')
            length++;
    saveFiles->length = length;

    rewinddir(dir);
    int index = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.')
            saveFiles->data[index] = entry->d_name;
        index++;
    }
    
    closedir(dir);
    return true;
}

void saveGame(const struct Piece *board, const int *whiteRecord, const int *blackRecord, bool isWhiteTurns)
{
    char name[256];
    printf("Please input the name of the save file: ");
    scanf("%s", name);
    FILE *fp = NULL;
    char filepath[INPUT_BUFFER_SIZE] = { "" };
    strcat(filepath, "./save/");
    strcat(filepath, name);
    if ((fp = fopen(filepath, "w")) != NULL) 
    {
        fprintf(fp, "------------------------SNAPSHOT-----------------------\n");
        fprintf(fp, "Black left: ♙ x%d ♘ x%d ♗ x%d ♖ x%d ♕ x%d ♔ x%d\n",
           blackRecord[Pawn],
           blackRecord[Knight],
           blackRecord[Bishop],
           blackRecord[Rook],
           blackRecord[Queen],
           blackRecord[King]);
        fprintf(fp, "White left: ♟︎ x%d ♞ x%d ♝ x%d ♜ x%d ♛ x%d ♚ x%d\n",
            whiteRecord[Pawn],
            whiteRecord[Knight],
            whiteRecord[Bishop],
            whiteRecord[Rook],
            whiteRecord[Queen],
            whiteRecord[King]);
        fprintf(fp, "\t  _________________________\n");
        for (int y = 0; y < BOARD_SIZE; y++)
        {
            fprintf(fp, "\t%d |", (9 - (y + 1)));
            for (int x = 0; x < BOARD_SIZE; x++)
            {
                int index = y * BOARD_SIZE + x;
                switch (board[index].type)
                {
                case Pawn:
                    board[index].isWhite ? fprintf(fp, "♟︎ ") : fprintf(fp, "♙ ");
                    break;
                case Knight:
                    board[index].isWhite ? fprintf(fp, "♞ ") : fprintf(fp, "♘ ");
                    break;
                case Bishop:
                    board[index].isWhite ? fprintf(fp, "♝ ") : fprintf(fp, "♗ ");
                    break;
                case Rook:
                    board[index].isWhite ? fprintf(fp, "♜ ") : fprintf(fp, "♖ ");
                    break;
                case Queen:
                    board[index].isWhite ? fprintf(fp, "♛ ") : fprintf(fp, "♕ ");
                    break;
                case King:
                    board[index].isWhite ? fprintf(fp, "♚ ") : fprintf(fp, "♔ ");
                    break;
                default:
                    if (y % 2)
                        index % 2 ? fprintf(fp, "__") : fprintf(fp, "##");
                    else
                        index % 2 ? fprintf(fp, "##") : fprintf(fp, "__");
                    break;
                }
                fprintf(fp, "|");
            }
            fprintf(fp, "\n");
        }
        fprintf(fp, "\t   a  b  c  d  e  f  g  h\n\n");
        
        fprintf(fp, "--------------------------DATA-------------------------\n");
        fprintf(fp, "#ISWHITETURNS\n");
        fprintf(fp, "%d\n", isWhiteTurns);

        fprintf(fp, "#WHITERECORD\n");
        for (int i = 0; i < NumberOfChessType; i++) 
            fprintf(fp, "%d\n", whiteRecord[i]);

        fprintf(fp, "\n#BLACKRECORD\n");
        for (int i = 0; i < NumberOfChessType; i++) 
            fprintf(fp, "%d\n", blackRecord[i]);

        fprintf(fp, "\n#BOARD\n");
        // fprintf(fp, "type\tisWhite\n");
        for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) 
            fprintf(fp, "%d\t%d\n", board[i].type, board[i].isWhite);

        fclose(fp);
    }
    else 
    {
        printf("Cannot write %s", filepath);
    }
}

void loadGame(const char *fileName, struct Piece *board, int *whiteRecord, int *blackRecord, bool *isWhiteTurns)
{
    FILE *fp = NULL;
    char filepath[INPUT_BUFFER_SIZE] = { "" };
    strcat(filepath, "./save/");
    strcat(filepath, fileName);
    if ((fp = fopen(filepath, "r")) != NULL) 
    {
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
        while ((read = getline(&line, &len, fp)) != -1 && strcmp(line, "#BOARD\n") != 0) 
        {
            // printf("Retrieved line of length %zu:\n", read);
            // printf("%s", line);
        }
        for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) 
        {
            fscanf(fp, "%d", &board[i].type);
            fscanf(fp, "%d", &board[i].isWhite);
        }

        rewind(fp);
        while ((read = getline(&line, &len, fp)) != -1 && strcmp(line, "#ISWHITETURNS\n") != 0) 
        {
            // printf("Retrieved line of length %zu:\n", read);
            // printf("%s", line);
        }
        fscanf(fp, "%d", isWhiteTurns);

        rewind(fp);
        while ((read = getline(&line, &len, fp)) != -1 && strcmp(line, "#WHITERECORD\n") != 0) 
        {
            // printf("Retrieved line of length %zu:\n", read);
            // printf("%s", line);
        }
        for (int i = 0; i < NumberOfChessType; i++) 
            fscanf(fp, "%d", &whiteRecord[i]);

        rewind(fp);
        while ((read = getline(&line, &len, fp)) != -1 && strcmp(line, "#BLACKRECORD\n") != 0) 
        {
            // printf("Retrieved line of length %zu:\n", read);
            // printf("%s", line);
        }
        for (int i = 0; i < NumberOfChessType; i++) 
            fscanf(fp, "%d", &whiteRecord[i]);

        fclose(fp);
    }
    else 
    {
        printf("Cannot read %s", filepath);
    }
}