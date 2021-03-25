#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include "fileManagement.h"
#include "game.h"

bool getSaveFiles(const char **saveFiles)
{
    struct dirent *entry;
    DIR *dir = opendir(SAVE_FOLDER_PATH); // open the save folder
    if (!dir) 
        return false;

    // store all file names in save folder to 'saveFiles'
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

// save the game to a file
void saveGame(const GameProps *game)
{
    // ask the player to input the name of the save file or input 'cancel' to cancel
    char name[BUFFER_SIZE] = { '\0' };
    printf("------------------------------------------------------------------------\n");
    printf("Please input the name of the save file (or input 'cancel' to cancel): ");
    scanf("%s", name);
    if (strcmp(name, "cancel") == 0) 
        return;

    FILE *fp = NULL;
    char filepath[BUFFER_SIZE] = { "" };
    // connect SAVE_FOLDR_PATH and user input file name and store the result to variable 'filepath'
    strcat(filepath, SAVE_FOLDER_PATH);
    strcat(filepath, name);
    // chcek if we can successfully open filepath
    if ((fp = fopen(filepath, "w")) != NULL) 
    {
        // -----------------store the game data to filepath---------------
        fprintf(fp, "------------------------SNAPSHOT-----------------------\n");
        fprintf(fp, "Black left: px%d nx%d bx%d rx%d qx%d kx%d\n",
            game->blackRecord[Pawn],
            game->blackRecord[Knight],
            game->blackRecord[Bishop],
            game->blackRecord[Rook],
            game->blackRecord[Queen],
            game->blackRecord[King]);
        fprintf(fp, "White left: Px%d Nx%d Bx%d Rx%d Qx%d Kx%d\n",
            game->whiteRecord[Pawn],
            game->whiteRecord[Knight],
            game->whiteRecord[Bishop],
            game->whiteRecord[Rook],
            game->whiteRecord[Queen],
            game->whiteRecord[King]);
        fprintf(fp, "\t  _________________________________\n");
        for (int y = 0; y < BOARD_SIZE; y++)
        {
            fprintf(fp, "\t%d |", (9 - (y + 1)));
            for (int x = 0; x < BOARD_SIZE; x++)
            {
                int index = y * BOARD_SIZE + x;
                switch (game->board[index].type)
                {
                case Pawn:
                    game->board[index].isWhite ? fprintf(fp, " P ") : fprintf(fp, " p ");
                    break;
                case Knight:
                    game->board[index].isWhite ? fprintf(fp, " N ") : fprintf(fp, " n ");
                    break;
                case Bishop:
                    game->board[index].isWhite ? fprintf(fp, " B ") : fprintf(fp, " b ");
                    break;
                case Rook:
                    game->board[index].isWhite ? fprintf(fp, " R ") : fprintf(fp, " r ");
                    break;
                case Queen:
                    game->board[index].isWhite ? fprintf(fp, " Q ") : fprintf(fp, " q ");
                    break;
                case King:
                    game->board[index].isWhite ? fprintf(fp, " K ") : fprintf(fp, " k ");
                    break;
                default:
                    if (y % 2)
                        index % 2 ? fprintf(fp, "___") : fprintf(fp, "###");
                    else
                        index % 2 ? fprintf(fp, "###") : fprintf(fp, "___");
                    break;
                }
                fprintf(fp, "|");
            }
            fprintf(fp, "\n");
        }
        fprintf(fp, "\t    a   b   c   d   e   f   g   h\n\n");
        
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

// load the game from a save file
void loadGame(const char *filename, GameProps *game)
{
    FILE *fp = NULL;
    char filepath[BUFFER_SIZE] = { "" };
    strcat(filepath, SAVE_FOLDER_PATH);
    strcat(filepath, filename);
    if ((fp = fopen(filepath, "r")) != NULL) 
    {
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
        // read the file line by line until we reach the line '#BOARD'
        while ((read = getline(&line, &len, fp)) != -1 && strcmp(line, "#BOARD\n") != 0) 
            ; // null statement

        // store the data to game->board from file
        for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) 
        {
            int int_type = 0;
            int int_isWhite = 0;
            fscanf(fp, "%d", &int_type);
            fscanf(fp, "%d", &int_isWhite);
            game->board[i].type = (enum ChessType)int_type;
            game->board[i].isWhite = (bool)int_isWhite;
        }

        rewind(fp); // get back to the beginning of the file
        // read the file line by line until we reach the line '#ISWHITETURNS'
        while ((read = getline(&line, &len, fp)) != -1 && strcmp(line, "#ISWHITETURNS\n") != 0) 
            ; // null statement

        // store the data to game->isWhiteTurns from file
        int int_isWhiteTurns = 0;
        fscanf(fp, "%d", &int_isWhiteTurns);
        game->isWhiteTurns = (bool)int_isWhiteTurns;

        rewind(fp); // get back to the beginning of the file
        // read the file line by line until we reach the line '#WHITERECORD'
        while ((read = getline(&line, &len, fp)) != -1 && strcmp(line, "#WHITERECORD\n") != 0) 
            ; // null statement 

        // store the data to game->whiteRecord from file
        for (int i = 0; i < NumberOfChessType; i++) 
            fscanf(fp, "%d", &game->whiteRecord[i]);

        rewind(fp); // get back to the beginning of the file
        // read the file line by line until we reach the line '#BLACKRECORD'
        while ((read = getline(&line, &len, fp)) != -1 && strcmp(line, "#BLACKRECORD\n") != 0) 
            ; // null statement 

        // store the data to game->blackRecord from file
        for (int i = 0; i < NumberOfChessType; i++) 
            fscanf(fp, "%d", &game->blackRecord[i]);

        fclose(fp);
    }
    else 
    {
        printf("Cannot read %s\n", filepath);
    }
}