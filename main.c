#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h> 

#include "sdl_utils.h"
#include "dynamics.h"
#include "trajectories.h"
#include "game.h"
#include "objectives.h"

int main(int argc, char* args[])
{
    // printf("Starting game\n");

    // Initialize SDL
    if (!init_sdl())
    {
        printf("Failed to initialize SDL\n");
        return -1;
    }

    // Initial multi-agent system
    if (init_state_list() == NULL)
    {
        printf("Failed to initialize SMA\n");
    }

    // Start the timer
    init_timer();

    render_screen();

    render_pause();

    SDL_RenderPresent(screen);

    // Main loop
    while (!QUIT)
    {
        // Event loop
        while (SDL_PollEvent(&event))
        {
            handle_events();
        }

        // Rendering loop
        if(!GAME_PAUSED)
        {
            forward();

            render_screen();

            check_objectives();            

            SDL_RenderPresent(screen); 
        }       

        // SDL_RenderPresent(screen); 
    }

    quit_game();

    return 0;
}