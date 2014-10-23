# Makefile
TAR = test
CC = gcc -O6 -Wall
L = -lm -libary
OBJS = main.o iwmatrix.o
SHAD = -fPIC -shared
LIB = libiwmatrix.so

all : $(TAR) lib
$(TAR): $(OBJS)
	  $(CC) -o $@ $(OBJS) $(L)
clean:
	  rm -f $(OBJS) $(LIB) $(TAR) *~ *.bak

.c.o:	  $(CC) -c $<om

lib:
	$(CC) $(SHAD) -o $(LIB) iwmatrix.c $(L)

main.o: main.h iwmatrix.o
iwmatrix.o: iwmatrix.h
