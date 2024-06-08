CC = gcc
EXEC = my_add
OBJS = my_add.o
COMP_FLAG = -ansi -Wall -pedantic
DEBUG_FLAG = -g

$(EXEC): $(OBJS)
	$(CC) $(COMP_FLAG) $(DEBUG_FLAG) $(OBJS) -o $@

my_scalar.o: my_add.c
	$(CC) -c $(COMP_FLAG) $*.c -o $@

clean:
	-rm $(OBJS)