#include <stdio.h>
#include <conio.h>

int main() {
    while (1) {
        if (kbhit()){
            printf("%i", getch());
        }
    }
    return 0;
}