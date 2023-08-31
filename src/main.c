#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#include "snake_queue.h"
#include "replace_char.h"

void render_board();
void replace_square(Location squareLocation, char newSquare);
void handle_movement(int *direction);

Location spawn_apple(Snake *snake);
bool apple_eaten(Snake snake, Location *apple);

bool body_collided(Snake *snake);
bool border_collided(Snake *snake, int direction);

int main() {
    // hides cursor. Small delay because with no delay hiding sometimes fails
    Sleep(50);
    printf("\033[?25l\n\n");
    
    srand(time(0));
    render_board();

    // goes 1 up 1 left so caret isn't outside map, then saves so we can always get back here
    printf("\033[1A\033[1D");
    printf("\033[s");

    // initialize snake, must set head and tail to NULL because of garbage values
    Snake snake = {NULL, NULL};
    Location firstPart = {COLS-1, ROWS-1};
    add_body_part(&snake, firstPart);
    replace_square(snake.head->location, SNAKE_BODY_SYMBOL);

    Location apple = spawn_apple(&snake);
    int direction = UP;

    while (1) {
        Sleep(65);
        handle_movement(&direction);
        if (border_collided(&snake, direction)) {
            break;
        }

        move_head(&snake, direction);
        bool appleChanged = apple_eaten(snake, &apple);
        if (!appleChanged) {
            move_tail(&snake);
        }

        if (body_collided(&snake)) {
            break;
        }
    }

    // show cursor and go down a line with caret
    printf("\033[?25h");
    printf("\033[1B");
    
    printf("\nGame over! Your score is %i\n", snake.length-1);

    return EXIT_SUCCESS;
}

void render_board() {
    // +2 accounts for square brackets border "[]"
    for (int i = 0; i < COLS+2; i++) {
        printf("=");
    }
    printf("\n");

    for (int i = 0; i < ROWS; i++) {
        printf("[");
        for (int j = 0; j < COLS; j++) {
            putchar(EMPTY_SYMBOL);
        }
        printf("]\n");
    }
    
    for (int i = 0; i < COLS+2; i++) {
        printf("=");
    }
}

void handle_movement(int *direction) {
    // no press detected
    if (!kbhit()) {
        return;
    }
    /* conio.h detects arrow keys by sending two characters. First is a 0 and second is a real number
    which might represent a normal character in most circumstances.
    That's why we return/skip if we don't get the special signal (int 0)
    */
    char keyPressed = getch();
    if (keyPressed != 0) {
        return;
    }
    // first char was special signal (int 0) so now we detect 2nd char
    char actualKey = getch();
    switch (actualKey) {
        case UP:
            if (*direction != DOWN) {
                *direction = UP;
            }
            break;
        case DOWN:
            if (*direction != UP) {
                *direction = DOWN;
            }
            break;
        case RIGHT:
            if (*direction != LEFT) {
                *direction = RIGHT;
            }
            break;
        case LEFT:
            if (*direction != RIGHT) {
                *direction = LEFT;
            }
            break;
    }
}

Location spawn_apple(Snake *snake) {
    // we don't use memcpy because this function relies on recursion, so we store original head instead
    BodyPart *originalHead = snake->head;
    Location apple = {rand()%(COLS), rand()%(ROWS)};

    while (snake->head != NULL) {
        if (snake->head->location.x == apple.x && snake->head->location.y == apple.y) {
            // change back to original head, recurse till we find a valid apple location
            snake->head = originalHead;
            apple = spawn_apple(snake);
            break;
        }
        snake->head = snake->head->next;
    }
    // we altered the real snake->head, so we change it back
    snake->head = originalHead;
    replace_square(apple, APPLE_SYMBOL);
    return apple;
}

bool apple_eaten(Snake snake, Location *apple) {
    if (snake.head->location.x == apple->x && snake.head->location.y == apple->y) {
        *apple = spawn_apple(&snake);
        return true;
    }
    return false;
}

bool body_collided(Snake *snake) {
    Snake *tmp1, *tmp2;

    memcpy(tmp1, snake, sizeof(snake));
    memcpy(tmp2, snake, sizeof(snake));

    // checks if any body part collides with another body part
    while (tmp1->head->next != NULL) {
        while (tmp2->head->next != NULL) {
            tmp2->head = tmp2->head->next;
            if (tmp1->head->location.x == tmp2->head->location.x && tmp1->head->location.y == tmp2->head->location.y) {
                return true;
            }
        }
        tmp1->head = tmp1->head->next;
        tmp2->head = tmp1->head->next;
    }
    return false;
}

bool border_collided(Snake *snake, int direction) {
    Location headLocation = snake->head->location;

    // predicts next location
    switch (direction) {
        case UP: headLocation.y--; break;
        case DOWN: headLocation.y++; break;
        case RIGHT: headLocation.x++; break;
        case LEFT: headLocation.x--; break;
    }
    // check if we'll collide
    if (headLocation.x < 0 || headLocation.x >= COLS) {
        return true;
    }
    if (headLocation.y < 0 || headLocation.y >= ROWS) {
        return true;
    }
    return false;
}