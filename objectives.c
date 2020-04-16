#include "objectives.h"
#include "dynamics.h"
#include "sdl_utils.h"
#include "trajectories.h"
#include "game.h"

#include <SDL2/SDL.h>
#include <stdbool.h>

const double UNIQUE_OBJECTIVE_CENTER = 2.0;
const double UNIQUE_OBJECTIVE_RADIUS = 0.25;

bool GAME_WON = false;

void check_objectives()
{
    if(unique_objective_achieved() && !GAME_WON) 
    {
        GAME_WON = true;
        GAME_PAUSED = true;
        render_win();                
    }
}

bool unique_objective_achieved()
{
    for(int i = 1; i < NB_AGENTS; i++)
    {
        if(state_list->state[i] > UNIQUE_OBJECTIVE_CENTER + UNIQUE_OBJECTIVE_RADIUS)
        {
            return false;
        }

        if(state_list->state[i] < UNIQUE_OBJECTIVE_CENTER - UNIQUE_OBJECTIVE_RADIUS)
        {
            return false;
        }
    }

    return true;
}

void render_unique_objective()
{
    double max, min;
    initial_min_max(&min,&max);

    double ky = (SCREEN_HEIGHT-2.0*TOP_BOT_MARGIN)/(max-min);
    double ay = -ky;
    double by = TOP_BOT_MARGIN + max*ky;
    // y vers le haut dans la simu, vers le bas dans sdl

    SDL_Rect rect;
    rect.x = TRAJECTORY_WIDTH + RANGE_EDGE_WIDTH;
    rect.y = (int) (ay*(UNIQUE_OBJECTIVE_CENTER+UNIQUE_OBJECTIVE_RADIUS)+by);
    rect.h = (int) (2.0*ky*UNIQUE_OBJECTIVE_RADIUS);
    rect.w = SCREEN_WIDTH - rect.x;

    SDL_SetRenderDrawColor(screen, 0x00, 0x00, 0xFF, 0x88);
    SDL_RenderFillRect(screen,&rect);
}

void render_win()
{
    int width = 100;
    
    SDL_Rect rect;
    rect.x = SCREEN_WIDTH/2 - width/2;
    rect.y = SCREEN_HEIGHT/2 - width/2;
    rect.h = width;
    rect.w = width;

    SDL_SetRenderDrawColor(screen, 0x00, 0x00, 0x00, 0x88);
    SDL_RenderFillRect(screen,&rect);
}