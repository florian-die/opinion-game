#ifndef __GAME__
#define __GAME__

#include <SDL2/SDL.h>
#include <stdbool.h> 

// Game state
extern bool GAME_PAUSED;
extern bool QUIT;

// Generic SDL Event for PollEvent loop
extern SDL_Event event;

void handle_events();

void handle_joy_axis();

void handle_joy_buttons();

void render_screen();

void render_pause();

void quit_game();



#endif