CC=clang++
CPPFLAGS=-c -g -Wall -std=c++20
LDFLAGS=-g

SRC=$(wildcard src/*.cpp)
BLD=$(subst src, bld, $(SRC))
OBJ=$(subst .cpp,.o, $(BLD))

all: sim

sim: $(OBJ)
	$(CC) $(LDFLAGS) -o sim $(OBJ)

$(OBJ): bld/%.o: src/%.cpp
	$(CC) $(CPPFLAGS) -o $@ $<
