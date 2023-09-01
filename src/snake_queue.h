#ifndef SNAKE_QUEUE_H
#define SNAKE_QUEUE_H

#define ROWS 20
#define COLS 30

#define SNAKE_BODY_SYMBOL 'O'
#define APPLE_SYMBOL '*'
#define EMPTY_SYMBOL '.'

#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75

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
void move_head(Snake *snake, int direction);

void free_snake(Snake *snake);

#endif