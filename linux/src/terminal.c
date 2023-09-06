#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/select.h>
#include <unistd.h>
#include <time.h>

#include "constants.h"

struct termios originalTerminal;

/*
 * Sleeps for miliseconds using nanosleep by multiplying with 1,000,000.
 */
void sleep_ms(long ms) {
    if (ms < 0) {
        return;
    }
    struct timespec sleepTime = {.tv_sec=ms / 1000, .tv_nsec=(ms%1000) * 1000000};
    nanosleep(&sleepTime, NULL);
}

/*
 * Uses the globally stored original terminal to go back to normal one and shows cursor again.
 */
static void reset_terminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &originalTerminal);
    printf("\033[?25h\033[1B");
}

/*
 * Sets up a snake game with cursor options and renders the board.
 */
void set_snake_terminal() {
    // hide cursor. Small delay because it sometimes fails to hide cursor otherwise
    sleep_ms(50);
    printf("\033[?25l\n\n");
    struct termios term;

    // Save the terminal attributes so we can restore them later.
    tcgetattr(STDIN_FILENO, &originalTerminal);
    atexit(reset_terminal);

    // Remove canonical mode and remove echo
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 1;
    term.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
}

/*
 * Switches directions whenever you hit an arrow key.
 * It does so with by detecting keys using termios.h, sys/select.h and reading from stdin.
 */
void handle_movement(Direction *direction) {
    // Hanging struct to pass to select. Set to 0 so we don't hang if player didn't press anything.
    struct timeval waitTime = {.tv_sec=0, .tv_usec=0};
    fd_set fd;

    FD_ZERO(&fd);
    FD_SET(STDIN_FILENO, &fd);
    select(STDIN_FILENO + 1, &fd, NULL, NULL, &waitTime);
    if (!FD_ISSET(STDIN_FILENO, &fd)) {
        return; // player didn't press anything
    }

    switch (fgetc(stdin)) {
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
