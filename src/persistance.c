#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include "persistance.h"
#include "game.h"

bool getSaveFiles(const char **saveFiles)
{
    struct dirent *entry;
    DIR *dir = opendir(SAVE_FOLDER_PATH);
    if (!dir) 
        return false;

    int index = 0;
    while ((entry = readdir(dir)) != NULL) 
    {
        if (entry->d_name[0] != '.')
        {
            saveFiles[index] = entry->d_name;
            index++;
        } 
    }
    
    closedir(dir);
    return true;
}

void saveGame(const GameProps *game)
{
    char name[256];
    printf("Please input the name of the save file: ");
    scanf("%s", name);
    FILE *fp = NULL;
    char filepath[INPUT_BUFFER_SIZE] = { "" };
    strcat(filepath, SAVE_FOLDER_PATH);
    strcat(filepath, name);
    if ((fp = fopen(filepath, "w")) != NULL) 
    {
        fprintf(fp, "------------------------SNAPSHOT-----------------------\n");
        fprintf(fp, "Black left: ♙ x%d ♘ x%d ♗ x%d ♖ x%d ♕ x%d ♔ x%d\n",
            game->blackRecord[Pawn],
            game->blackRecord[Knight],
            game->blackRecord[Bishop],
            game->blackRecord[Rook],
            game->blackRecord[Queen],
            game->blackRecord[King]);
        fprintf(fp, "White left: ♟︎ x%d ♞ x%d ♝ x%d ♜ x%d ♛ x%d ♚ x%d\n",
            game->whiteRecord[Pawn],
            game->whiteRecord[Knight],
            game->whiteRecord[Bishop],
            game->whiteRecord[Rook],
            game->whiteRecord[Queen],
            game->whiteRecord[King]);
        fprintf(fp, "\t  _________________________\n");
        for (int y = 0; y < BOARD_SIZE; y++)
        {
            fprintf(fp, "\t%d |", (9 - (y + 1)));
            for (int x = 0; x < BOARD_SIZE; x++)
            {
                int index = y * BOARD_SIZE + x;
                switch (game->board[index].type)
                {
                case Pawn:
                    game->board[index].isWhite ? fprintf(fp, "♟︎ ") : fprintf(fp, "♙ ");
                    break;
                case Knight:
                    game->board[index].isWhite ? fprintf(fp, "♞ ") : fprintf(fp, "♘ ");
                    break;
                case Bishop:
                    game->board[index].isWhite ? fprintf(fp, "♝ ") : fprintf(fp, "♗ ");
                    break;
                case Rook:
                    game->board[index].isWhite ? fprintf(fp, "♜ ") : fprintf(fp, "♖ ");
                    break;
                case Queen:
                    game->board[index].isWhite ? fprintf(fp, "♛ ") : fprintf(fp, "♕ ");
                    break;
                case King:
                    game->board[index].isWhite ? fprintf(fp, "♚ ") : fprintf(fp, "♔ ");
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
        fprintf(fp, "%d\n", game->isWhiteTurns);

        fprintf(fp, "\n#WHITERECORD\n");
        for (int i = 0; i < NumberOfChessType; i++) 
            fprintf(fp, "%d\n", game->whiteRecord[i]);

        fprintf(fp, "\n#BLACKRECORD\n");
        for (int i = 0; i < NumberOfChessType; i++) 
            fprintf(fp, "%d\n", game->blackRecord[i]);

        fprintf(fp, "\n#BOARD\n");
        // fprintf(fp, "type\tisWhite\n");
        for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) 
            fprintf(fp, "%d\t%d\n", game->board[i].type, game->board[i].isWhite);

        fclose(fp);
    }
    else 
    {
        printf("Cannot write %s", filepath);
    }
}

void loadGame(const char *filename, GameProps *game)
{
    FILE *fp = NULL;
    char filepath[INPUT_BUFFER_SIZE] = { "" };
    strcat(filepath, SAVE_FOLDER_PATH);
    strcat(filepath, filename);
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
            fscanf(fp, "%d", &game->board[i].type);
            fscanf(fp, "%d", &game->board[i].isWhite);
        }

        rewind(fp);
        while ((read = getline(&line, &len, fp)) != -1 && strcmp(line, "#ISWHITETURNS\n") != 0) 
        {
            // printf("Retrieved line of length %zu:\n", read);
            // printf("%s", line);
        }
        fscanf(fp, "%d", &game->isWhiteTurns);

        rewind(fp);
        while ((read = getline(&line, &len, fp)) != -1 && strcmp(line, "#WHITERECORD\n") != 0) 
        {
            // printf("Retrieved line of length %zu:\n", read);
            // printf("%s", line);
        }
        for (int i = 0; i < NumberOfChessType; i++) 
            fscanf(fp, "%d", &game->whiteRecord[i]);

        rewind(fp);
        while ((read = getline(&line, &len, fp)) != -1 && strcmp(line, "#BLACKRECORD\n") != 0) 
        {
            // printf("Retrieved line of length %zu:\n", read);
            // printf("%s", line);
        }
        for (int i = 0; i < NumberOfChessType; i++) 
            fscanf(fp, "%d", &game->blackRecord[i]);

        fclose(fp);
    }
    else 
    {
        printf("Cannot read %s\n", filepath);
    }
}