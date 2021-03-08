#pragma once

#include <stdbool.h>
#define INPUT_BUFFER_SIZE 256

typedef struct
{
    char from[3];
    char to[3];
    bool invalid;
    bool quit;
    bool restart;
    bool promote;
    bool castle;
    bool save;
    bool load;
} InputProps;

InputProps getUserInput(bool isWhiteTurns);