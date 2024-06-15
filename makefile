#CC = gcc
#EXEC = main
#OBJS = main.o
#COMP_FLAG = -ansi -Wall -pedantic
#DEBUG_FLAG = -g
#
#$(EXEC): $(OBJS)
#	$(CC) $(COMP_FLAG) $(DEBUG_FLAG) $(OBJS) -o $@
#
#my_scalar.o: main.c
#	$(CC) -c $(COMP_FLAG) $*.c -o $@
#
#clean:
#	-rm $(OBJS)

assembler: main.o first_run.o
	gcc -g -ansi -Wall -pedantic main.o first_run.o -lm -o assembler

main.o: main.c first_run.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o

first_run.o: first_run.c first_run.h assembler.h
	gcc -c -ansi -Wall -pedantic first_run.c -o first_run.o

