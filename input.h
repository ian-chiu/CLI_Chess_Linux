#pragma once

#include <stdbool.h>
#define INPUT_BUFFER_SIZE 256

struct InputProps
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
};

struct InputProps getUserInput(bool isWhiteTurns);