// FIXME:   en passant can only apply 'immediately after' move 2 square from start position
//          maybe add a history system
// TODO:    castle the king
#include <stdio.h>
#include <ev.h>
#include <stdlib.h>
#include "GameState.h"
#include "History.h"
#include "game.h"
#include "fileManagement.h"

// all watcher callbacks have a similar signature
// this callback is called when data is readable on stdin
static void
stdin_cb(EV_P_ ev_io *w, int revents)
{
    puts("stdin ready");
    // for one-shot events, one must manually stop the watcher
    // with its corresponding stop function.
    ev_io_stop(EV_A_ w);

    // this causes all nested ev_run's to stop iterating
    ev_break(EV_A_ EVBREAK_ALL);
}

// another callback, this time for a time-out
static void
timeout_cb(EV_P_ ev_timer *w, int revents)
{
    *(bool*)(w->data) = true;
    ev_sleep(1);
    ev_timer_stop(EV_A_ w);
    ev_break(EV_A_ EVBREAK_ALL);
}

static void
countdown_cb(EV_P_ ev_timer *w, int revents)
{
    printf("%d sec left...\n", *(int*)w->data - 1);
    *(int*)w->data -= 1;
    ev_timer_again(loop, w);
}

int main()
{
    GameState *gameState = GameState__construct();
    History *history = History__construct();
    init(gameState, history);

    struct ev_loop *loop = EV_DEFAULT;

    float round_time = 11.0f;

    ev_timer timer_watcher;
    timer_watcher.repeat = round_time;
    bool timesup = false;
    timer_watcher.data = &timesup;
    ev_init(&timer_watcher, timeout_cb);

    int counter = round_time;
    ev_timer countdown_watcher;
    countdown_watcher.repeat = 1.0;
    countdown_watcher.data = &counter;
    ev_init(&countdown_watcher, countdown_cb);

    ev_io stdin_watcher;
    ev_io_init(&stdin_watcher, stdin_cb, 0, EV_READ);

    displayStartMenu();

    while (!gameState->finish)
    {
        render(gameState);

        if (hasWinner(gameState))
        {
            processWin(gameState, history);
        }
        else
        {
            ev_io_start(loop, &stdin_watcher);
            timer_watcher.repeat = counter;
            ev_timer_again(loop, &timer_watcher);
            ev_timer_again(loop, &countdown_watcher);
            ev_run(loop, 0);

            if (timesup) {
                timesup = false;
                counter = round_time;
                gameState->isWhiteTurns = !gameState->isWhiteTurns;
                continue;
            }

            InputProps input = getUserInput();

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
                move(&input, gameState, history);
            }
            else
            {
                promptInvalid();
            }
        }
    }

    displayGoodbye();
    GameState__destroy(gameState);
    History__destroy(history);
    return 0;
}