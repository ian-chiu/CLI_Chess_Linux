// FIXME:   en passant can only apply 'immediately after' move 2 square from start position
//          maybe add a history system
// TODO:    castle the king
// FIXME:   fix countdown when input somethin
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "GameState.h"
#include "History.h"
#include "game.h"
#include "fileManagement.h"
#include "EventState.h"

#include <sys/time.h>
#include <unistd.h>

// all watcher callbacks have a similar signature
// this callback is called when data is readable on stdin
static void
stdin_cb(struct ev_loop *loop, ev_io *w, int revents)
{
    EventState *eventStatePtr = (EventState*)w->data;
    char temp[BUFFER_SIZE] = {'\0'};

    struct timeval start, end;
    gettimeofday(&start, NULL);
    wgetstr(eventStatePtr->inputWindow, temp);
    gettimeofday(&end, NULL);
    double time_taken;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;
    if (time_taken >= eventStatePtr->countdown || eventStatePtr->countdown - time_taken < 1.0)
        eventStatePtr->timesup = true;
    else
    {
        strcpy(eventStatePtr->inputStr, temp);
        eventStatePtr->countdown -= time_taken;
    }

    ev_io_stop(loop, w);
    ev_break(loop, EVBREAK_ALL);
}

static void
countdown_cb(struct ev_loop *loop, ev_timer *w, int revents)
{
    EventState *eventStatePtr = (EventState*)w->data;
    eventStatePtr->countdown--;
    if (eventStatePtr->countdown < 1.0f) 
    {
        eventStatePtr->timesup = true;
        ev_timer_stop(loop, w);
        ev_break(loop, EVBREAK_ALL);
    }
    render(eventStatePtr->gameStatePtr, eventStatePtr);
    refresh();
    wrefresh(eventStatePtr->inputWindow);
    ev_timer_again(loop, w);
}

int main()
{
    initscr();			
	keypad(stdscr, TRUE);

    // int nScreenWidth = 100;
    int nScreenHeight = 16;
    WINDOW *inputWindow = newwin(6, COLS, nScreenHeight, 0);

    curs_set(0);

    GameState *gameState = GameState__construct();
    History *history = History__construct();
    init(gameState, history);

    // ---------------------------------------------
    EventState eventState;

    eventState.countdown = ROUND_TIME;
    eventState.timesup = false;
    eventState.gameStatePtr = gameState;
    // eventState.gameWindow = gameWindow;
    eventState.inputWindow = inputWindow;
    for (int i = 0; i < BUFFER_SIZE; i++)
        eventState.inputStr[i] = '\0';

    ev_timer countdown_watcher;
    countdown_watcher.repeat = 1.0;
    countdown_watcher.data = &eventState;
    ev_init(&countdown_watcher, countdown_cb);

    ev_io stdin_watcher;
    stdin_watcher.data = &eventState;
    ev_io_init(&stdin_watcher, stdin_cb, STDIN_FILENO, EV_READ);
    // -------------------------------------------------

    // displayStartMenu();

    while (!gameState->finish)
    {
        clear();
        wclear(inputWindow);

        render(gameState, &eventState);
        wprintw(inputWindow, gameState->isWhiteTurns ? "White turns: \n" : "Black turns: \n");

        refresh();
        wrefresh(inputWindow);

        if (hasWinner(gameState))
        {
            processWin(inputWindow, gameState, history);
        }
        else
        {
            struct ev_loop *loop = EV_DEFAULT;
            ev_io_start(loop, &stdin_watcher);
            ev_timer_again(loop, &countdown_watcher);
            ev_run(loop, 0);

            if (eventState.timesup) 
            {
                eventState.timesup = false;
                eventState.countdown = ROUND_TIME;
                gameState->isWhiteTurns = !gameState->isWhiteTurns;
                ev_timer_stop(loop, &countdown_watcher);
                continue;
            }

            InputProps input = getUserInput(eventState.inputWindow, eventState.inputStr);

            if (input.quit)
            {
                processQuit(inputWindow, gameState);
            }
            else if (input.restart)
            {
                processRestart(inputWindow, gameState, history);
            }
            else if (input.save)
            {
                saveGame(gameState);
            }
            else if (input.load)
            {
                processLoad(gameState, history);
            }
            else if (input.undo)
            {
                processUndo(gameState, history);
            }
            else if (input.redo)
            {
                processRedo(gameState, history);
            }
            else if (input.help)
            {
                displayHelp();
            }
            else if (!input.invalid)
            {
                if (movePiece(inputWindow, &input, gameState, history))
                    eventState.countdown = ROUND_TIME;
            }
            else
            {
                promptInvalid(inputWindow);
            }
        }
    }

    displayGoodbye();
    endwin();
    GameState__destroy(gameState);
    History__destroy(history);
    return 0;
}