CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -Werror -g

SRC_DIR=src
BIN=bin

EXE=snake

SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(patsubst $(SRC_DIR)/%.c, $(BIN)/%.o, $(SRCS))

.PHONY: all clean

all: $(EXE)

$(EXE): $(BIN) $(OBJS)
	$(CC) -o $(BIN)/$@ $(OBJS)

$(BIN)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(BIN):
	mkdir $(BIN)

clean:
	@if exist $(BIN) rmdir /s /q $(BIN)