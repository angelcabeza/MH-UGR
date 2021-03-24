HOME     = .
BIN      = $(HOME)/bin
INCLUDE  = $(HOME)/include
SRC      = $(HOME)/src
OBJ      = $(HOME)/obj
DATOS	   = $(HOME)/conjunto_datos_PAR_2020-21

all: $(BIN)/par

$(BIN)/par : $(OBJ)/PAR.o $(OBJ)/datos.o
	g++ $(OBJ)/PAR.o $(OBJ)/datos.o -o $(BIN)/par -std=c++11

$(OBJ)/PAR.o: $(SRC)/PAR.cpp
	g++ $^ -I$(INCLUDE) -c -o $@ -std=c++11

$(OBJ)/datos.o: $(SRC)/datos.cpp
	g++ $^ -I$(INCLUDE) -c -o $@ -std=c++11

clean:
	-rm $(OBJ)/*.o
	-rm $(BIN)/*