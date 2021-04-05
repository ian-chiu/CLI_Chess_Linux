#pragma once

#include <stdbool.h>
#include <ncurses.h>

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
    bool redo;
    bool undo;
    bool replay;
    double timer;
} InputProps;

InputProps getUserInput(char *inputStr);