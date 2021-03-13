#pragma once

#include <stdbool.h>

#define BUFFER_SIZE 256

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
    bool help;
} InputProps;

InputProps getUserInput(bool isWhiteTurns);