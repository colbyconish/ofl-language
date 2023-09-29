CXX= g++
LD = g++

SRC=src
BIN=bin
OBJ=obj
LIB=lib
EXE=ofc
INC=include

DEV=debug
PROD=release
PRODFLAG=OFL_PRODUCTION

CXXFLAGS= $(addprefix -I, $(INC)) --std=c++20 -O0 -g3
LDFLAGS=-L$(LIB) -LC:/"Third Party Libraries"/lib -lswe

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(OBJ)/main.o $(OBJ)/lexer.o $(OBJ)/parser.o $(OBJ)/assembler.o $(OBJ)/executor.o
	$(LD) $(LDFLAGS) $^ -o $(BIN)/$(DEV)/$(EXE) 

$(OBJ)/%_prod.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -D$(PRODFLAG) -c $< -o $@

prod: $(OBJ)/main_prod.o $(OBJ)/lexer_prod.o $(OBJ)/parser_prod.o $(OBJ)/assembler_prod.o $(OBJ)/executor_prod.o
	$(LD) $(LDFLAGS) $^ -o $(BIN)/$(PROD)/$(EXE)

run: all
	./$(BIN)/$(DEV)/$(EXE) run main.xx

.PHONY: debug
debug: all
	gdb bin/debug/ofc.exe -ex "b main" -ex "layout src" -ex "r run main.xx" 

clean:
	@rm -rf $(OBJ)/*
	@rm -rf $(BIN)/$(DEV)/*
	@rm -rf $(BIN)/$(PROD)/*

