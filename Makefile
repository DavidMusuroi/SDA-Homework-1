CC = gcc
FLAGS = -Wall
PROGS = tema1

build:
	$(CC) tema1.c -o $(PROGS) $(FLAGS)

run:
	./$(PROGS)

clean:
	rm -f $(PROGS)