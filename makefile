HOME     = .
BIN      = $(HOME)/bin
INCLUDE  = $(HOME)/include
SRC      = $(HOME)/src
OBJ      = $(HOME)/obj
DATOS	   = $(HOME)/conjunto_datos_PAR_2020-21

all: $(BIN)/par

$(BIN)/par : $(OBJ)/PAR.o $(OBJ)/datos.o
	g++ $(OBJ)/PAR.o $(OBJ)/datos.o -o $(BIN)/par

$(OBJ)/PAR.o: $(SRC)/PAR.cpp $(OBJ)/datos.o
	g++ -c $^ -I$(INCLUDE) -o $@

$(OBJ)/datos.o: $(SRC)/datos.cpp $(INCLUDE)/datos.h
	g++ -c $^ -I$(INCLUDE) -o $@

clean:
	-rm $(OBJ)/*.o
	-rm $(BIN)/*