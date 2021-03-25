// FIXME:   en passant can only apply 'immediately after' move 2 square from start position 
//          maybe add a history system 
// TODO:    castle the king
#include <stdio.h>
#include "game.h"
#include "fileManagement.h"

int main()
{
    GameProps game;

    init(&game);
    startMenu();

    while (!game.finish)
    {
        render(&game);

        if (hasWinner(&game))
        {
            process_win(&game);
        }
        else
        {
            InputProps input = getUserInput(game.isWhiteTurns);

            if (input.quit)
            {
                process_quit(&game.finish);
            }
            else if (input.restart)
            {
                process_restart(&game);
            }
            else if (input.save)
            {
                saveGame(&game);
            }
            else if (input.load)
            {
                process_load(&game);
            }
            else if (input.help)
            {
                help();
            }
            else if (!input.invalid)
            {
                move(&input, &game);
            }
            else
            {
                prompt_invalid();
            }
        }
    }

    goodbye();
    return 0;
}