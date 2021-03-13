#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <assert.h>
#include "input.h"
#include "game.h"

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
    result.help = false;

    isWhiteTurns ? printf("White turns: ") : printf("Black turns: ");

    // ---------------input 1------------------
    char input1[BUFFER_SIZE];
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

    // check if input1 is 'help'
    if (strcmp(input1, "help") == 0)
    {
        result.help = true;
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