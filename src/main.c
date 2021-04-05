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
#include <signal.h>

volatile sig_atomic_t flag = 0;
void press_Ctrl_C(int sig){ // can be called asynchronously
    flag = 1; // set flag
}

// all watcher callbacks have a similar signature
// this callback is called when data is readable on stdin
static void
stdin_cb(struct ev_loop *loop, ev_io *w, int revents)
{
    int y, x;
    getyx(stdscr, y, x);
    move(0, 0);
    printw("You are in INPUT MODE! The timer is still ticking.\nTo see the timer, please press 'enter' with no input.");
    move(y, x);

    EventState *eventStatePtr = (EventState*)w->data;
    char temp[BUFFER_SIZE] = {'\0'};

    struct timeval start, end;
    gettimeofday(&start, NULL);
    getstr(temp);
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
    ev_timer_again(loop, w);
}

int main()
{
    signal(SIGINT, press_Ctrl_C); 

    GameState *gameState = GameState__construct();
    History *history = History__construct();
    init(gameState, history);
    keypad(stdscr, TRUE);

    // ---------------------------------------------
    EventState eventState;

    eventState.roundTime = 60.0;
    eventState.countdown = eventState.roundTime;
    eventState.timesup = false;
    eventState.gameStatePtr = gameState;
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

    displayStartMenu();

    initscr();	

    while (!gameState->finish)
    {
        render(gameState, &eventState);
        refresh();

        if (hasWinner(gameState))
        {
            processWin(gameState, history);
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
                eventState.countdown = eventState.roundTime;
                gameState->isWhiteTurns = !gameState->isWhiteTurns;
                ev_timer_stop(loop, &countdown_watcher);
                switchTurns(gameState->isWhiteTurns);
                continue;
            }

            InputProps input = getUserInput(eventState.inputStr);

            if (input.quit)
            {
                processQuit(gameState);
            }
            else if (input.restart)
            {
                processRestart(gameState, history);
            }
            else if (input.save)
            {
                saveGame(gameState);
            }
            else if (input.load)
            {
                processLoad(gameState, history);
                eventState.countdown = eventState.roundTime;
            }
            else if (input.undo)
            {
                processUndo(gameState, history);
                eventState.countdown = eventState.roundTime;
            }
            else if (input.redo)
            {
                processRedo(gameState, history);
                eventState.countdown = eventState.roundTime;
            }
            else if (input.help)
            {
                displayHelp();
            }
            else if (input.replay)
            {
                processReplay(history);
            }
            else if (input.timer != 0.0)
            {
                eventState.roundTime = input.timer;
                eventState.countdown = eventState.roundTime + 1;
            }
            else if (!input.invalid)
            {
                if (movePiece(&input, gameState, history))
                {
                    eventState.countdown = eventState.roundTime;
                    switchTurns(gameState->isWhiteTurns);
                }
            }
            else
            {
                promptInvalid();
            }
        }

        clrtobot();

        if(flag)
        {
            endwin();
            GameState__destroy(gameState);
            History__destroy(history);
            return 0;
        }  
    }

    displayGoodbye();
    endwin();
    GameState__destroy(gameState);
    History__destroy(history);
    return 0;
}