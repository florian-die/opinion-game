#include "trajectories.h"
#include "dynamics.h"
#include "state_list.h"
#include "sdl_utils.h"

#include <SDL2/SDL.h>
#include <stddef.h>

const double PERSISTENCE_DURATION = 7.0;
int TRAJECTORY_WIDTH = 0.0; // set in render_trajectories()

const int TOP_BOT_MARGIN = 10;

const int RANGE_ALPHA = 0x2F;
const int RANGE_EDGE_WIDTH = 3;

int count_persitent_points()
{
    struct state_list_t *list = state_list;
    
    if(list == NULL) return 0;

    if(PERSISTENCE_DURATION <= 0.0) return 0;
    
    int count = 0;
    
    double current_time = list->time;

    double limit_time = current_time - PERSISTENCE_DURATION;

    if(limit_time < 0.0) limit_time = 0.0;

    while(list->time >= limit_time)
    {
        count++;
        list = list->next;
        if(list == NULL) break;
    }

    return count;
}

void render_trajectories()
{
    // Number of points of each trajectory
    int nb_points = count_persitent_points();
    if(nb_points == 0) return;

    // Array of points to draw
    SDL_Point *points = malloc((nb_points+1)*sizeof(SDL_Point));

    // Current state
    struct state_list_t * state = NULL;
    double current_time = state_list->time;
    double max, min;
    initial_min_max(&min,&max);

    TRAJECTORY_WIDTH = 2*SCREEN_WIDTH/3;

    double ky = (SCREEN_HEIGHT-2.0*TOP_BOT_MARGIN)/(max-min);
    double ay = -ky;
    double by = TOP_BOT_MARGIN + max*ky;

    double kx = TRAJECTORY_WIDTH / PERSISTENCE_DURATION;
    double ax = kx;
    double bx = -(current_time-PERSISTENCE_DURATION)*kx;

    int range;

    for(int i = 0; i < NB_AGENTS; i++)
    {   
        state = state_list;

        points[0].x = TRAJECTORY_WIDTH;
        points[0].y = (int) (ay*state->state[i] + by);
        
        for(int k = 1; k < nb_points; k++)
        {
            state = state->next;
            points[k].x = (int) (ax*state->time + bx);  
            points[k].y = (int) (ay*state->state[i] + by);
        }

        points[nb_points].x = 0;
        points[nb_points].y = (int) (ay * state->state[i] + by);

        if (i == 0) 
        {
            // Set player trajectories color
            SDL_SetRenderDrawColor(screen, 0xFF, 0x00, 0x00, 0xFF);
            SDL_RenderDrawLines(screen,points,nb_points+1);

            // Bold player trajectory
            for(int k = 0; k <= nb_points; k++) points[k].y--;
            SDL_RenderDrawLines(screen,points,nb_points+1);
            for(int k = 0; k <= nb_points; k++) points[k].y += 2;
            SDL_RenderDrawLines(screen,points,nb_points+1);
        }
        else
        {
            // Set agents trajectories color
            SDL_SetRenderDrawColor(screen, 0x00, 0x00, 0xFF, 0xFF);
            SDL_RenderDrawLines(screen,points,nb_points+1);
        }

        // Draw range of non player agents
        if(i == 0)
        {
            // Set translucent line for influence range
            SDL_SetRenderDrawColor(screen, 0xFF, 0x00, 0x00, RANGE_ALPHA);

            range = (int) (PLAYER_INFLUENCE_RANGE*ky); // range in pixels
        }
        else
        {
            // Set translucent line for influence range
            SDL_SetRenderDrawColor(screen, 0x00, 0x00, 0xFF, RANGE_ALPHA);

            range = (int) (AGENT_INFLUENCE_RANGE*ky); // range in pixels
        }        

        if (range > 0)
        {
            // Draw range line
            SDL_RenderDrawLine(screen,points[0].x,points[0].y+range,points[0].x,points[0].y-range);

            // Draw range edges
            SDL_RenderDrawLine(screen,
                points[0].x-RANGE_EDGE_WIDTH, // x1
                points[0].y-range, // y1
                points[0].x+RANGE_EDGE_WIDTH, // x2
                points[0].y-range); // y2
            SDL_RenderDrawLine(screen,
                points[0].x-RANGE_EDGE_WIDTH,
                points[0].y+range,
                points[0].x+RANGE_EDGE_WIDTH,
                points[0].y+range);
        }
        
    }
    
    free(points);
}
