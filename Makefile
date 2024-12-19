CC = g++
LIBS = -lm
SRC = src
OBJ = obj
INC = include
BIN = bin
OBJS = $(OBJ)/main.o $(OBJ)/dense.o $(OBJ)/inverted.o $(OBJ)/mem_address.o $(OBJ)/hier2.o $(OBJ)/hier3.o
HDRS =  $(INC)/dense.h $(INC)/inverted.h $(INC)/mem_address.h $(INC)/hier2.h $(INC)/hier3.h
CFLAGS = -Wall -c -I$(INC)

EXE = $(BIN)/main

$(BIN)/main: $(OBJS)
	$(CC) -w -o $(BIN)/main $(OBJS) $(LIBS)

$(OBJ)/main.o: $(HDRS) $(SRC)/main.c
	$(CC) $(CFLAGS) -w -o $(OBJ)/main.o $(SRC)/main.c 
	
$(OBJ)/dense.o: $(HDRS) $(SRC)/dense.c
	$(CC) $(CFLAGS) -w -o $(OBJ)/dense.o $(SRC)/dense.c 
	
$(OBJ)/inverted.o: $(HDRS) $(SRC)/inverted.c
	$(CC) $(CFLAGS) -w -o $(OBJ)/inverted.o $(SRC)/inverted.c 
	
$(OBJ)/mem_address.o: $(HDRS) $(SRC)/mem_address.c
	$(CC) $(CFLAGS) -w -o $(OBJ)/mem_address.o $(SRC)/mem_address.c

$(OBJ)/hier2.o: $(HDRS) $(SRC)/hier2.c
	$(CC) $(CFLAGS) -w -o $(OBJ)/hier2.o $(SRC)/hier2.c

$(OBJ)/hier3.o: $(HDRS) $(SRC)/hier3.c
	$(CC) $(CFLAGS) -w -o $(OBJ)/hier3.o $(SRC)/hier3.c

clean:
	rm -f $(EXE) $(OBJS)