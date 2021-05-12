HOME     = .
BIN      = $(HOME)/bin
INCLUDE  = $(HOME)/include
SRC      = $(HOME)/src
OBJ      = $(HOME)/obj
LIB		 = $(HOME)/lib
DATOS	   = $(HOME)/conjunto_datos_PAR_2020-21

all: clean $(BIN)/par

$(BIN)/par : $(OBJ)/PAR.o $(OBJ)/practica1.o $(OBJ)/random.o $(OBJ)/timer.o
	g++ -O3 $^ -o $(BIN)/par -std=c++11

$(OBJ)/PAR.o: $(SRC)/PAR.cpp
	g++ -O3 $^ -I$(INCLUDE) -c -o $@ -std=c++11

$(OBJ)/practica1.o: $(SRC)/practica1.cpp
	g++ -O3 $^ -I$(INCLUDE) -c -o $@ -std=c++11

$(OBJ)/random.o: $(SRC)/random.cpp
	g++ -O3 $^ -I$(INCLUDE) -c -o $@ -std=c++11

$(OBJ)/timer.o: $(SRC)/timer.c
	g++ -O3 $^ -I$(INCLUDE) -c -o $@

clean:
	-rm $(OBJ)/*.o
	-rm $(BIN)/par