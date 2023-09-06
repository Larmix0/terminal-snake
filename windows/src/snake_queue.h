#ifndef SNAKE_QUEUE_H
#define SNAKE_QUEUE_H

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

// returns an initialized snake with one body part in the corner of the map.
Snake *snake_init(Location firstPart);

// Puts a new body part as the head in passed location.
void add_body_part(Snake *snake, Location location);

// Removes the snake's tail.
void remove_body_part(Snake *snake);

/*
 * move_tail: Simply removes tail.
 * move_head: Puts a new head of the snake depending on the direction.
 * These 2 together create the illusion of movement in the snake.
 */
void move_tail(Snake *snake);
void move_head(Snake *snake, Direction direction);

// Deallocates all memory in snake.
void free_body_parts(Snake *snake);

#endif
