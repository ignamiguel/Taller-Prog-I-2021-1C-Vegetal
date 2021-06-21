
CXX			:= g++
CXX_FLAGS	:= -Wall -Wextra -std=c++11 -g
LD_FLAGS	:= -lSDL2 -lSDL2_image -ljsoncpp -pthread
 
BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib
OBJ		:= obj
 
LIBRARIES	:=
EXECUTABLE	:= main
 
 
all: $(BIN)/$(EXECUTABLE)
 
run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)
 
$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp  $(SRC)/view/*.cpp $(SRC)/model/*.cpp $(SRC)/model/mario/*.cpp $(SRC)/model/stage/*.cpp $(SRC)/controller/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES) $(LD_FLAGS)
 
clean:
	-rm $(BIN)/*
