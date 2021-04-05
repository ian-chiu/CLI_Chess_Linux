#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include <ctype.h>
#include "input.h"
#include "gameProps.h"

#include <ncurses.h>

InputProps getUserInput(char *inputStr)
{
    InputProps result;
    result.quit = false;
    result.restart = false;
    result.invalid = false;
    result.promote = false;
    result.castle = false;
    result.save = false;
    result.load = false;
    result.help = false;
    result.redo = false;
    result.undo = false;
    result.replay = false;
    result.timer = 0.0;

    refresh();

    // ---------------input 1------------------
    char input1[BUFFER_SIZE] = { '\0' };
    char input2[BUFFER_SIZE] = { '\0' };
    sscanf(inputStr, "%s %s", input1, input2);

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

    // check if input1 is 'help'
    if (strcmp(input1, "help") == 0)
    {
        result.help = true;
        return result;
    }

    // check if input1 is 'undo'
    if (strcmp(input1, "undo") == 0)
    {
        result.undo = true;
        return result;
    }

    // check if input1 is 'redo'
    if (strcmp(input1, "redo") == 0)
    {
        result.redo = true;
        return result;
    }

    // check if input1 is 'redo'
    if (strcmp(input1, "replay") == 0)
    {
        result.replay = true;
        return result;
    }

    if (strcmp(input1, "timer") == 0)
    {
        result.timer = atof(input2);
    }

    if (strlen(input1) == 2 && isalpha(input1[0]) && islower(input1[0]) && isdigit(input1[1]))
    {
        // store first input position to result
        result.from[0] = input1[0];
        result.from[1] = input1[1];
        result.from[2] = '\0';

        if (result.from[0] - 'a' >= BOARD_SIZE || result.from[1] - '0' > BOARD_SIZE || result.from[1] == '0')
        {
            result.invalid = true;
           printw("ERROR: Input out of range!\n");
            return result;
        }
    }
    else 
    {
        result.invalid = true;
        return result;
    }

    // ---------------input 2------------------
    // char input2[BUFFER_SIZE] = { '\0' };
    // scanw("%s", input2);

    // check if input2 is 'promote'
    if (strcmp(input2, "promote") == 0)
    {
        result.promote = true;
        return result;
    }

    // check if input2 is 'castle', THE GAME HAS NOT SUPPORTED CASTLE YET
    if (strcmp(input2, "castle") == 0)
    {
        result.castle = true;
        return result;
    }
    
    if (strlen(input2) == 2 && isalpha(input2[0]) && islower(input2[0]) && isdigit(input2[1]))
    {
        // store second input position to result 'to'
        result.to[0] = input2[0];
        result.to[1] = input2[1];
        result.to[2] = '\0';

        if (result.to[0] - 'a' >= BOARD_SIZE || result.to[1] - '0' > BOARD_SIZE  || result.to[1] == '0')
        {
            result.invalid = true;
           printw("ERROR: Input out of range!\n");
        }
    }
    else 
    {
        result.invalid = true;
    }

    return result;
}