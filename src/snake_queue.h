#ifndef SNAKE_QUEUE_H
#define SNAKE_QUEUE_H

typedef struct {
    int x;
    int y;
} Location;

typedef struct BodyPart {
    Location location;
    struct BodyPart *next;
} BodyPart;

typedef struct {
    BodyPart *head;
    BodyPart *tail;
    int length;
} Snake;

void add_body_part(Snake *snake, Location location);
void remove_body_part(Snake *snake);

void move_tail(Snake *snake);
void move_head(Snake *snake, Direction direction);

void free_snake(Snake *snake);

#endif