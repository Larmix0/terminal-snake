CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -Werror -g

SRCS=$(wildcard src/*.c)
OBJS=*.o

BIN=snake

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $@ $^

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c $^

clean:
	del $(OBJS) $(BIN).exe