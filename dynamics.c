#include "dynamics.h"

#include "sdl_utils.h"

#include <SDL2/SDL.h>
#include <stdlib.h> 
#include <stdio.h>
#include <math.h> 

const double FORWARD_TIME_STEP = 0.01;

const double PLAYER_INFLUENCE_GAIN = 0.5;
const double PLAYER_INFLUENCE_RANGE = 0.5;
const double AGENT_INFLUENCE_GAIN = 0.05;
const double AGENT_INFLUENCE_RANGE = 0.0;

const unsigned int NB_AGENTS = 6;
const double INITIAL_STATE[] = {2.0, 0.0, 0.5, 0.9, 3.5, 4.0};

double current_player_speed = 0.0;
const double MAX_PLAYER_SPEED = 3.0;

influence* player_influence = influence_HK;
influence* agent_influence = influence_HK;

struct state_list_t * state_list = NULL;

// Non-const copy of initial state
double* copy_initial_state()
{
    double *state = malloc(NB_AGENTS*sizeof(double));
    if(state == NULL) return NULL;

    for(int i = 0; i < NB_AGENTS; i++)
        state[i] = INITIAL_STATE[i];

    return state;
}

// Initialize linked list of states with initial conditions
void* init_state_list()
{    
    state_list = free_state_list(state_list);
    
    state_list = add_state(NULL,0.0,copy_initial_state());
    
    return state_list;
}

// Integrate dynamics for a fixed step time
double* forward_step(double *state, double step)
{
    double* new_state = system_dynamics(state);

    double min, max;
    initial_min_max(&min,&max);

    for(int i = 0; i < NB_AGENTS; i++)
    {
        // Euler step
        new_state[i] *= step;
        new_state[i] += state[i];
        
        // Bound state
        if(new_state[i] > max) new_state[i] = max;
        if(new_state[i] < min) new_state[i] = min;
    }

    return new_state;
}

// Integrate dynamics for any duration with small steps stored in the global linked list
void forward()
{
    // Compute trajectories
    timer.current_tick = SDL_GetTicks();
    double elapsed_time = (double)(timer.current_tick-timer.previous_tick)/1000.0;

    state_list = forward_duration(state_list,elapsed_time);

    timer.previous_tick = timer.current_tick;
    // print_present_state();    
}

// Integrate dynamics for any duration with small steps stored in a linked list
struct state_list_t * forward_duration(struct state_list_t *state, double duration)
{
    if(duration <= 0.0) return state;
    
    if(duration > FORWARD_TIME_STEP)
    {
        state = add_state(state, // head of list
            state->time+FORWARD_TIME_STEP, // time
            forward_step(state->state,FORWARD_TIME_STEP)); // state

        return forward_duration(state,duration-FORWARD_TIME_STEP);
    }
    else
    {
        state = add_state(state, // head of list
            state->time+duration, // time
            forward_step(state->state,duration)); // state

        return state;
    }
}

// Compute system dynamics
double* system_dynamics(double *state)
{
    double* dynamics = malloc(NB_AGENTS*sizeof(double));
    if(dynamics == NULL)
    {
        return NULL;
    }

    double diff = 0.0;

    // Set speed of the player
    dynamics[0] = current_player_speed;

    // Compute speed of the free agents
    for(int i = 1; i < NB_AGENTS; i++)
    {
        // Player interactions
        diff = state[0]-state[i];
        dynamics[i] = PLAYER_INFLUENCE_GAIN * player_influence(diff,PLAYER_INFLUENCE_RANGE);

        // Inter-agents interactions
        for(int j = 1; j < NB_AGENTS; j++)
        {
            if(i != j)
            {
                diff = state[j]-state[i];
                dynamics[i] += AGENT_INFLUENCE_GAIN * agent_influence(diff,AGENT_INFLUENCE_RANGE);
            }
        }
    }

    return dynamics;
}

// HK influence function
double influence_HK(double x, double range)
{
    if(fabs(x) <= range) return x; else return 0.0;
}

// Uniform influence function
double influence_uniform(double x, double range)
{
    if(fabs(x) <= range) return 1.0; else return 0.0;
}

// Return min/max initial opinions
void initial_min_max(double *min, double *max)
{
    for(int i = 0; i < NB_AGENTS; i++)
    {
        if(INITIAL_STATE[i] < *min) *min = INITIAL_STATE[i];
        if(INITIAL_STATE[i] > *max) *max = INITIAL_STATE[i];
    }
}

void print_present_state()
{
    printf("Current state (t=%fs) = {",state_list->time);
    for(int i = 0; i < NB_AGENTS; i++)
    {
        printf("%f",state_list->state[i]);
        if(i < NB_AGENTS-1)
            printf(", ");
    }
    printf("}\n");
}