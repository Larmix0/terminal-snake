# Snake game in terminal (no GUI)
This is a small side-project I did when learning c to get more comfortable in memory management and pointers. There are no dependencies on this project other than builtin libraries

# Compile
Currently you have to run the following inside the src directory (I'll likely update it later with a makefile, but i've been too lazy to install it):

```
gcc -o snake main.c replace_char.c snake_queue.c
```

Then you start the game by running:

```
./snake
```

![game example](example_snake.png)