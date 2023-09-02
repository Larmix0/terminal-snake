#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#include "constants.h"
#include "snake_queue.h"
#include "locations.h"

void render_board();
void handle_movement(Direction *direction);

Location spawn_apple(Snake *snake);
bool apple_eaten(Snake snake, Location *apple);

bool body_collided(Snake *snake);
bool border_collided(Snake *snake, Direction direction);

/*
 * Main function that runs snake.
 */
int main() {
    // hides cursor. Small delay because it sometimes fails to hide it otherwise
    Sleep(50);
    printf("\033[?25l\n\n");

    srand(time(0));
    render_board();

    // goes 1 up 1 left so caret isn't outside map, then saves so we can always get back here
    printf("\033[1A\033[1D");
    printf("\033[s");

    // initialize snake, must set head and tail to NULL because of garbage values
    Snake snake = {NULL, NULL, 0};
    Location firstPart = {COLS - 1, ROWS - 1};
    add_body_part(&snake, firstPart);
    replace_location(snake.head->location, SNAKE_BODY_SYMBOL);

    Location apple = spawn_apple(&snake);
    Direction direction = UP;

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

    int score = snake.length - 1;
    int highScore = 0; // default if read file doesn't exist
    FILE *scoreRead = fopen("high_score.txt", "r");
    if (scoreRead != NULL) {
        fscanf(scoreRead, "%d", &highScore);
    }
    fclose(scoreRead);

    int highestScore = score > highScore ? score : highScore;
    FILE *scoreWrite = fopen("high_score.txt", "w");
    fprintf(scoreWrite, "%d", highestScore);
    fclose(scoreWrite);

    bool newPersonalBest = score > highScore ? true : false;
    if (newPersonalBest) {
        printf("\nCongratulations, your new personal best is %i\n", highestScore);
    }
    else {
        printf("\nYour score was %i, and your personal best is %i\n", score, highestScore);
    }

    free_snake(&snake);
    return EXIT_SUCCESS;
}

/*
 * Prints an empty board to the screen at the beginning of the game.
 */
void render_board() {
    // +2 accounts for square brackets border "[]"
    for (int i = 0; i < COLS + 2; i++) {
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

    for (int i = 0; i < COLS + 2; i++) {
        printf("=");
    }
}

/*
 * Switches directions whenever you hit an arrow key.
 * It does so with by detecting arrow keys using conio.h by sending two characters.
 * First is always a 0 (to signify a special number's coming) then the second is a real number
 * which would've represented a normal character in without the special 0 first.
 * That's why we return/skip if we don't get the special signal (int 0)
 */
void handle_movement(Direction *direction) {
    // no press detected
    if (!kbhit()) {
        return;
    }
    char keyPressed = getch();
    if (keyPressed != 0) {
        return;
    }
    // first char was special signal (0) so now we detect 2nd char
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

/*
 * Writes an apple at a random, valid location on the screen.
 * It recursively calls itself if the randomly generated position is invalid,
 * and it does so forever until it finds a valid spot.
 * The function doesn't utilize memcpy because recursion kinda breaks it,
 * so we store the original head to switch back to it when we alter the real snake.
 */
Location spawn_apple(Snake *snake) {
    BodyPart *originalHead = snake->head;
    Location apple = {rand() % (COLS), rand() % (ROWS)};

    while (snake->head != NULL) {
        if (locations_match(snake->head->location, apple)) {
            // recurse till we find a valid apple location
            snake->head = originalHead;
            apple = spawn_apple(snake);
            break;
        }
        snake->head = snake->head->next;
    }
    // we altered the real snake->head, so we change it back
    snake->head = originalHead;
    replace_location(apple, APPLE_SYMBOL);
    return apple;
}

/*
 * Checks if apple was eaten by seeing if its location matches any of the snake's body parts.
 */
bool apple_eaten(Snake snake, Location *apple) {
    if (locations_match(snake.head->location, *apple)) {
        *apple = spawn_apple(&snake);
        return true;
    }
    return false;
}

/*
 * Checks if snake's head collided with any other body part.
 * It does so by keeping track of 2 pointers to check if any 2 body parts match in x and y.
 */
bool body_collided(Snake *snake) {
    BodyPart *bodyPtr1 = snake->head;
    BodyPart *bodyPtr2 = snake->head;

    // checks if any body part collides with another body part
    while (bodyPtr1->next != NULL) {
        while (bodyPtr2->next != NULL) {
            bodyPtr2 = bodyPtr2->next;

            if (locations_match(bodyPtr1->location, bodyPtr2->location)) {
                return true;
            }
        }
        bodyPtr1 = bodyPtr1->next;
        bodyPtr2 = bodyPtr1->next;
    }
    return false;
}

/*
 * checks if any body part collided with the border and returns result in a bool.
 */
bool border_collided(Snake *snake, Direction direction) {
    Location headLocation = snake->head->location;

    // predicts next location
    switch (direction) {
    case UP:
        headLocation.y--;
        break;
    case DOWN:
        headLocation.y++;
        break;
    case RIGHT:
        headLocation.x++;
        break;
    case LEFT:
        headLocation.x--;
        break;
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