#include <stdlib.h>

#include "snake_queue.h"
#include "replace_char.h"

void add_body_part(Snake *snake, Location location) {
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

void remove_body_part(Snake *snake) {
    // store head so we can deallocate
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

void move_tail(Snake *snake) {
    /* if our head collides with tail before we move it, 
    we pop the tail without removing it visually (since it's also the head for a frame)
    */
    if (snake->head->location.x == snake->tail->location.x && snake->head->location.y == snake->tail->location.y){
        remove_body_part(snake);
        return;
    }
    replace_square(snake->tail->location, EMPTY_SYMBOL);
    remove_body_part(snake);
}

void move_head(Snake *snake, int direction) {
    Location newLocation = {snake->head->location.x, snake->head->location.y};

    switch (direction) {
        case UP: newLocation.y--; break;
        case LEFT: newLocation.x--; break;
        case RIGHT: newLocation.x++; break;
        case DOWN: newLocation.y++; break;
    }
    add_body_part(snake, newLocation);
    replace_square(newLocation, SNAKE_BODY_SYMBOL);
}