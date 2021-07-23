CC = gcc
CFLAGS = -Wall -std=c99
LDLIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SRC_DIR = ./src
OBJ_DIR = ./obj
VPATH = $(SRC_DIR):$(OBJ_DIR)

SRC := $(notdir $(wildcard $(SRC_DIR)/*.c))
OBJ := $(SRC:.c=.o)


all: minesweeper

minesweeper: $(OBJ)
	$(CC) $(addprefix $(OBJ_DIR)/, $(OBJ)) -o $@ $(LDLIBS)

$(OBJ): %o: %c
	$(CC) $(CFLAGS) -c $< -o $(OBJ_DIR)/$@

clean:
	rm -rf $(OBJ_DIR)/* minesweeper
.PHONY: all clean
