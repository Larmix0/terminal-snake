#ifndef SNAKE_H
#define SNAKE_H

#include "constants.h"
#include "locations.h"

typedef struct BodyPart {
    Location location;
    struct BodyPart *next;
} BodyPart;

typedef struct {
    BodyPart *head;
    BodyPart *tail;
    int length;
} Snake;

// Puts a new body part as the head in passed location.
void add_body_part(Snake *snake, Location location);

// Removes the snake's tail.
void remove_body_part(Snake *snake);

// Higher level abstraction of remove_body_part that also updates the UI.
void move_tail(Snake *snake);

// Higher level abstraction of add_body_part that also updates the UI
void move_head(Snake *snake, Direction direction);

// Deallocates all memory in snake.
void free_snake(Snake *snake);

#endif