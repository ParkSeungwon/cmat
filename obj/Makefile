CC = g++
OBJ = $(wildcard *.o)
TOBJ = $(wildcard *.t)
EXE = $(patsubst %.x, ../%.x, $(wildcard *.x))
TST = $(patsubst %.tx, ../%.x, $(wildcard *.tx))

all : $(EXE) $(TST)

../%.x : %.x $(OBJ)
	$(CC) -o $@ $< $(OBJ) 

../%.x : %.tx $(TOBJ) $(OBJ)
	$(CC) -o $@ $< $(TOBJ) $(OBJ)

