SRC_DIR=src
LIB_DIR=lib
OBJ_DIR=obj
BIN_DIR=bin

CC=g++
CFLAGS=-Wall
LFLAGS=-Wall -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

INC= -I$(LIB_DIR)/glfw/include/ -Iinclude

EXECUTABLE=$(BIN_DIR)/pong

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS :=  $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

$(EXECUTABLE): $(OBJS)
	$(CC) $< -o $@ $(LFLAGS)

.PHONY: run
run: $(EXECUTABLE)
	./$(EXECUTABLE)

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(BIN_DIR)/*.out