CFLAG = -g -c -std=c++14 -fmax-errors=1 -pthread
INCLUDE = -I../ -I../src/ -I../framework/ -I../database/ -I../tls/ -I/usr/include/jsoncpp -I/usr/include/opencv
CC = g++
TEST_EXE = $(patsubst %.cpp, ../obj/%.tx, $(wildcard *.cpp))
TEST_OBJ = $(patsubst %.cc, ../obj/%.t, $(wildcard *.cc))

all : $(TEST_EXE) $(TEST_OBJ)

../obj/%.t : %.cc
	$(CC) $(INCLUDE) $(CFLAG) $< -o $@

../obj/%.tx : %.cpp
	$(CC) $(INCLUDE) $(CFLAG) $< -o $@
