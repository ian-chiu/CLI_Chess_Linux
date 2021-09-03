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
void stopProcessInterrupt(int sig) 
{ // can be called asynchronously
    flag = 1; // set flag
}

static void
stdin_cb(struct ev_loop *loop, ev_io *w, int revents)
{
    // get to the first line and print the message
    int y, x;
    getyx(stdscr, y, x);
    move(0, 0);
    printw("You are in INPUT MODE! The timer is still ticking.\nTo see the timer, please press 'enter' with no input.");
    move(y, x);

    // get our data pointer with type conversion
    EventState *eventStatePtr = (EventState*)w->data;
    char temp[BUFFER_SIZE] = {'\0'};

    // get the time taken by user
    struct timeval start, end;
    gettimeofday(&start, NULL);
    getstr(temp);
    gettimeofday(&end, NULL);
    double time_taken;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;

    // if time taken is greater than countdown time then time is up
    if (time_taken >= eventStatePtr->countdown)
        eventStatePtr->timesup = true;
    else
    {
        // store the user input string to data's inputStr, so we can check the input later
        strcpy(eventStatePtr->inputStr, temp);
        // subtract time taken from countdown time, so the time taken from the user can be considered
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
    
    // the accuracy of the time is 1 second, so if the countdowsn is less than 1 second we say the time is up
    if (eventStatePtr->countdown < 1.0f) 
    {
        eventStatePtr->timesup = true;
        ev_timer_stop(loop, w);
        ev_break(loop, EVBREAK_ALL);
    }
    render(eventStatePtr->gameStatePtr, eventStatePtr);
    refresh();
    w->repeat = 1.0;
    ev_timer_again(loop, w);
}

int main()
{
    signal(SIGINT, stopProcessInterrupt); 

    GameState *gameState = GameState__construct();
    History *history = History__construct();
    init(gameState, history);

    // initialize eventState (variables which is controled by events)
    EventState eventState;
    eventState.roundTime = 60.0;
    eventState.countdown = eventState.roundTime;
    eventState.timesup = false;
    eventState.gameStatePtr = gameState;
    for (int i = 0; i < BUFFER_SIZE; i++)
        eventState.inputStr[i] = '\0';

    // countdown watcher will trigger event every 1 second
    ev_timer countdownWatcher;
    countdownWatcher.repeat = 1.0;
    // let countdownWatcher's data pointer point to eventState, 
    // so when the event happens we can manipulate the data
    countdownWatcher.data = &eventState; 
    ev_init(&countdownWatcher, countdown_cb);

    ev_io stdinWatcher;
    // let stdinWatcher's data pointer point to eventState, 
    // so when the event happens we can manipulate the data
    stdinWatcher.data = &eventState;
    ev_io_init(&stdinWatcher, stdin_cb, STDIN_FILENO, EV_READ);

    displayStartMenu();

    initscr();	// start ncurses
    keypad(stdscr, TRUE); // ncurses - enable keypad

    while (!gameState->finish)
    {
        render(gameState, &eventState);
        refresh();

        if (hasWinner(gameState))
        {
            processWin(gameState, history);
            clear(); // clear the screnn
        }
        else
        {
            // reset the timer, and start the event loop
            struct ev_loop *loop = EV_DEFAULT;
            ev_io_start(loop, &stdinWatcher);
            ev_timer_again(loop, &countdownWatcher);
            ev_run(loop, 0);

            // if time's up, reset the event state and change to another player.
            if (eventState.timesup) 
            {
                eventState.timesup = false;
                eventState.countdown = eventState.roundTime;
                gameState->isWhiteTurns = !gameState->isWhiteTurns;
                ev_timer_stop(loop, &countdownWatcher);
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
            else if (input.timer)
            {
                if (input.timer > 10.0f && input.timer < 100000.0f) 
                {
                    eventState.roundTime = input.timer;
                    eventState.countdown = eventState.roundTime + 1;
                }
                else 
                    displayMessage("The timer must set greater than 10.0 and less than 100000 seconds!");
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

        // clear from the current line to the end of screen
        clrtobot();

        // if ctrl-c or ctrl-z is pressed, end ncurses and stop the game.
        // without doing this, terminal will be messed up.
        if(flag)
        {
            endwin();
            GameState__destroy(gameState);
            History__destroy(history);
            return 0;
        }  
    }

    displayGoodbye();
    endwin(); // end ncurses
    GameState__destroy(gameState);
    History__destroy(history);
    return 0;
}