CC = gcc
EXEC = main
OBJS = main.o
COMP_FLAG = -ansi -Wall -pedantic
DEBUG_FLAG = -g

$(EXEC): $(OBJS)
	$(CC) $(COMP_FLAG) $(DEBUG_FLAG) $(OBJS) -o $@

my_scalar.o: main.c
	$(CC) -c $(COMP_FLAG) $*.c -o $@

clean:
	-rm $(OBJS)