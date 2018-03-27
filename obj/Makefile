CC = g++
OBJ = $(wildcard *.o)
EXE = $(patsubst %.x, ../%.x, $(wildcard *.x))
TST = $(patsubst %.t, ../%.x, $(wildcard *.t))

all : $(EXE)
test : $(TST)

../%.x : %.x $(OBJ)
	$(CC) -o $@ $< $(OBJ) 

../%.x : %.t $(OBJ)
	$(CC) -o $@ $< $(OBJ)

