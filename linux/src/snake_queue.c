#include <stdlib.h>

#include "constants.h"
#include "snake_queue.h"
#include "locations.h"

static void add_body_part(Snake *snake, Location location);
static void remove_body_part(Snake *snake);

/*
 * Returns a dynamically allocated snake with the first body part made.
 */
Snake *snake_init(Location firstPart) {
    Snake *snake = malloc(sizeof(Snake));
    snake->head = NULL;
    snake->tail = NULL;
    snake->length = 0;

    add_body_part(snake, firstPart);
    replace_location(snake->head->location, SNAKE_BODY_SYMBOL);

    return snake;
}

/*
 * creates a new body part, and adds it as the new head.
 * It also has to handle having no body parts.
 * This could happen while moving with one body part
 * since we delete the head and then put it somewhere else in that case.
 */
static void add_body_part(Snake *snake, Location location) {
    // create new body part
    BodyPart *newBodyPart = malloc(sizeof(BodyPart));
    if (newBodyPart == NULL) {
        return;
    }
    newBodyPart->location = location;

    // there are body parts
    if (snake->head != NULL) {
        newBodyPart->next = snake->head;
        snake->head = newBodyPart;
    }
    // no body parts
    else {
        newBodyPart->next = NULL;
        snake->head = newBodyPart;
        snake->tail = newBodyPart;
    }
    snake->length++;
}

/*
 * Sets 2nd to last body part to be the new tail and deallocates the old tail.
 * If snake only has one body part then just make it store NULL.
 */
static void remove_body_part(Snake *snake) {
    // store head so we can deallocate if needed
    BodyPart *tmp = snake->head;

    // go to node before tail
    while (tmp->next != snake->tail) {
        tmp = tmp->next;
    }
    // snake only has one body part
    if (snake->head->next == NULL) {
        snake->head = NULL;
        snake->tail = NULL;
        free(tmp);
    }
    // snake has multiple body parts
    else {
        free(snake->tail);
        tmp->next = NULL;
        snake->tail = tmp;
    }
    snake->length--;
}

/*
 * If our head collides with tail before we move it,
 * we pop the tail without removing it visually (since it's also the head for a frame)
 */
void move_tail(Snake *snake) {
    if (locations_match(snake->head->location, snake->tail->location)) {
        remove_body_part(snake);
        return;
    }
    replace_location(snake->tail->location, EMPTY_SYMBOL);
    remove_body_part(snake);
}

/*
 * adds that body part in the appropriate direction and updates the UI.
 */
void move_head(Snake *snake, Direction direction) {
    Location newLocation = {snake->head->location.x, snake->head->location.y};

    switch (direction) {
    case UP: newLocation.y--; break;
    case LEFT: newLocation.x--; break;
    case RIGHT: newLocation.x++; break;
    case DOWN: newLocation.y++; break;
    }
    add_body_part(snake, newLocation);
    replace_location(newLocation, SNAKE_BODY_SYMBOL);
}

/*
 * Calls free on all allocated memory in the snake by doing it on all of the body parts.
 */
void free_body_parts(Snake *snake) {
    BodyPart *current = snake->head;
    BodyPart *next = NULL;

    // stores next body part and frees current one
    while (current->next != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    free(current); // free tail since its ->next is NULL
}
