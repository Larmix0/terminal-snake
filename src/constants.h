#ifndef CONSTANTS_H
#define CONSTANTS_H

#define ROWS 20
#define COLS 30

#define SNAKE_BODY_SYMBOL 'O'
#define APPLE_SYMBOL '*'
#define EMPTY_SYMBOL '.'

#define DELAY_MS 65

typedef enum {
    UP = 72,
    DOWN = 80,
    RIGHT = 77,
    LEFT = 75
} Direction;

#endif