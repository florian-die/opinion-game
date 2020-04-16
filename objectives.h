#ifndef __OBJECTIVES__
#define __OBJECTIVES__

#include <stdbool.h> 

const double UNIQUE_OBJECTIVE_CENTER;
const double UNIQUE_OBJECTIVE_RADIUS;

bool GAME_WON;

void check_objectives();

bool unique_objective_achieved();

void render_unique_objective();

void render_win();

#endif