#ifndef TERMINAL_H
#define TERMINAL_H

#include "constants.h"

// Stops execution of the program for miliseconds or seconds if ms > 1000
void sleep_ms(long ms);

// sets up the terminal for the snake game
void set_snake_terminal();

// reads if user pushed an arrow key
void handle_movement(Direction *direction);

// prints a default starting board
void render_board();

#endif
