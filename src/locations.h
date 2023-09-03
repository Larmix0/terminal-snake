#ifndef REPLACE_CHAR_H
#define REPLACE_CHAR_H

#include <stdbool.h>

typedef struct {
    int x;
    int y;
} Location;

// Replaces a square in the grid and returns to original location.
void replace_location(Location squareToReplace, char newSquare);

// Sees if passed locations have same x and y.
bool locations_match(Location location1, Location location2);
#endif
