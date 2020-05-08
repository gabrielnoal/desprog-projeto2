CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -Og
LFLAGS=
OBJCMD=$(CC) $(CFLAGS) $< -c
EXECMD=$(CC) $^ -o $@ $(LFLAGS)
OBJS=sort.o main.o
EXEC=main

all: $(EXEC)

sort.o: sort.c
	$(OBJCMD)

main.o: main.c sort.h
	$(OBJCMD)

main: sort.o main.o
	$(EXECMD)

clean:
	rm -f $(OBJS)

destroy: clean
	rm -f $(EXEC)
