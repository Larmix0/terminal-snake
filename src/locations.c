#include <stdio.h>
#include <stdbool.h>

#include "constants.h"
#include "locations.h"

/*
 * Because the ansi sequence automatically moves at least one (even if 0's provided),
 * we only move if we know the math expression's result will be 1 or higher
 */
void replace_location(Location squareToReplace, char newSquare) {
    if (squareToReplace.y != ROWS - 1) {
        printf("\033[%iA", ROWS - squareToReplace.y - 1); // go up rows
    }
    if (squareToReplace.x != COLS - 1) {
        printf("\033[%iD", COLS - squareToReplace.x - 1); // go right cols
    }
    // write char then go back to original/saved position
    printf("\b%c\033[u", newSquare);
}

/*
 * Returns bool that represents whether 2 locations hold the same x and y or not.
 */
bool locations_match(Location location1, Location location2) {
    return location1.x == location2.x && location1.y == location2.y;
}