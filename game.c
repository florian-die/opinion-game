#include "game.h"

#include "dynamics.h"
#include "trajectories.h"
#include "sdl_utils.h"
#include "objectives.h"

#include <SDL2/SDL.h>
#include <stddef.h>

// Game state
bool GAME_PAUSED = true; // Pause game if set to true
bool QUIT = false; // Exit program if set to true

// Generic SDL Event for PollEvent loop
SDL_Event event;

void handle_events()
{
    if(event.type == SDL_QUIT)
    {
        QUIT = true;
    }

    handle_joy_axis();

    handle_joy_buttons();
}

void handle_joy_axis()
{
    if(event.type == SDL_JOYAXISMOTION)
    {
        if(event.jaxis.axis == 1)
        {
            current_player_speed = - (double) (event.jaxis.value * MAX_PLAYER_SPEED) ;
            current_player_speed /= (double) MAX_JOYSTICK_AXIS_VALUE;
        }
        
        // printf("Event JoyAxisMotion {axis: %i,value: %i}\n",
        //     event.jaxis.axis, event.jaxis.value);
    }
}

void handle_joy_buttons()
{
    if(event.type == SDL_JOYBUTTONDOWN || event.type == SDL_JOYBUTTONUP)
    {
        // Pause
        if(event.jbutton.button == 7 && event.jbutton.state == SDL_PRESSED)
        {
            GAME_PAUSED = !GAME_PAUSED;

            if(GAME_PAUSED)
            {
                render_pause();

                SDL_RenderPresent(screen); 
            }
            else
            {
                timer.previous_tick = SDL_GetTicks();
            }
            
        }

        // Reset
        if(event.jbutton.button == 6 && event.jbutton.state == SDL_PRESSED)
        {
            init_state_list();

            init_timer();

            render_screen();

            render_pause();

            SDL_RenderPresent(screen); 

            GAME_PAUSED = true;

            GAME_WON = false;
        }

        // printf("Event JoyButton {button: %i, state: %i}\n",
        //     event.jbutton.button, event.jbutton.state);
    }
}

void render_screen()
{
    // Clear screen with white
    SDL_SetRenderDrawColor(screen, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(screen);

    // Render trajectories
    render_trajectories();

    render_unique_objective();

    // Update screen
    // SDL_SetRenderDrawBlendMode(screen, SDL_BLENDMODE_BLEND);
    // SDL_RenderPresent(screen);

    // Free too old states
    // shorten_state_list(state_list,timer.current_tick/1000.0-PERSISTENCE_DURATION);
}

void render_pause()
{
    SDL_SetRenderDrawColor(screen, 0x00, 0x00, 0x00, 0xAF);
    
    int width = 30;
    int height = 100;
    int space = 20;

    SDL_Rect rect;
    rect.w = width;
    rect.h = height;
    rect.x = SCREEN_WIDTH/2 + space/2;
    rect.y = SCREEN_HEIGHT/2 - height/2;
            
    SDL_RenderFillRect(screen, &rect);

    rect.x = rect.x - space - width;

    SDL_RenderFillRect(screen, &rect);
}

void quit_game()
{
    // Free trajectory memory
    free_state_list(state_list);

    quit_sdl();
}