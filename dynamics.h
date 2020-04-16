#ifndef __DYNAMICS__
#define __DYNAMICS__

#include "state_list.h"

const double FORWARD_TIME_STEP;

const double PLAYER_INFLUENCE_GAIN;
const double PLAYER_INFLUENCE_RANGE;
const double AGENT_INFLUENCE_GAIN;
const double AGENT_INFLUENCE_RANGE;

const unsigned int NB_AGENTS;
const extern double INITIAL_STATE[];

double current_player_speed;
const double MAX_PLAYER_SPEED;

struct state_list_t * state_list;

typedef double influence(double, double);
influence* player_influence;
influence* agent_influence;

// Non-const copy of initial state
double* copy_initial_state();

// Initialize linked list of states with initial conditions
void* init_state_list();

// Integrate dynamics for a small time step
double* forward_step(double *state, double step);

// Integrate dynamics with small steps and store them in the global linked list
void forward();

// Intergate dynamics for any duration and stores step in a linked list
struct state_list_t * forward_duration(struct state_list_t *state, double duration);

// Compute system dynamics
double* system_dynamics(double *state);

// HK influence function
double influence_HK(double x, double range);

// Uniform influence function
double influence_uniform(double x, double range);

// Return min/max initial opinions
void initial_min_max(double *min, double *max);

void print_present_state();

#endif