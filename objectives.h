#ifndef __OBJECTIVES__
#define __OBJECTIVES__

#include <stdbool.h> 

const extern double UNIQUE_OBJECTIVE_CENTER;
const extern double UNIQUE_OBJECTIVE_RADIUS;

bool extern GAME_WON;

void check_objectives();

bool unique_objective_achieved();

void render_unique_objective();

void render_win();

#endif