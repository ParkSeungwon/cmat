CC = g++
OBJ = $(wildcard *.o)
TOBJ = $(wildcard *.t)
EXE = $(patsubst %.x, ../%.x, $(wildcard *.x))
TST = $(patsubst %.tx, ../%.x, $(wildcard *.tx))
GLLIB = -lGLEW -lGLU -lGL -lglfw 
cvLIB = $(shell pkg-config opencv --libs)

all : $(EXE) $(TST)

../%.x : %.x $(OBJ)
	$(CC) -o $@ $< $(OBJ) $(GLLIB) $(cvLIB) -lpthread

../%.x : %.tx $(TOBJ) $(OBJ)
	$(CC) -o $@ $< $(TOBJ) $(OBJ) $(GLLIB) $(cvLIB) -lpthread

