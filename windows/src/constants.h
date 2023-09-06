#ifndef CONSTANTS_H
#define CONSTANTS_H

#define ROWS 20
#define COLS 30

#define SNAKE_BODY_SYMBOL 'O'
#define APPLE_SYMBOL '*'
#define EMPTY_SYMBOL '.'

#define DELAY_MS 65

typedef enum {
    UP = 119,
    DOWN = 115,
    RIGHT = 100,
    LEFT = 97
} Direction;

#endif
