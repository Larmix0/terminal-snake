#include <stdio.h>

#include "snake_queue.h"

void replace_square(Location squareLocation, char newSquare) {
    /* because the ansi sequence automatically moves at least one (even if 0's provided),
    we only move if we know the math expression's result will be 1 or higher
    */
    if (squareLocation.y != ROWS-1) {
        printf("\033[%iA", ROWS-squareLocation.y-1); // go up rows
    }
    if (squareLocation.x != COLS-1) {
        printf("\033[%iD", COLS-squareLocation.x-1); // go right cols
    }
    // write char then go back to original/saved position
    printf("\b%c", newSquare);
    printf("\033[u");
}