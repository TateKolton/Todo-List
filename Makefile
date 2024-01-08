CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS =

SRC = todo.c task.c saveList.c
OBJ = $(SRC:.c=.o)

all: todo

todo: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) todo

.PHONY: all clean
