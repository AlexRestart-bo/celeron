CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -g --verbose

SR = src
IN = inc
OB = obj
BN = bin

TARGET = $(BN)/str_study

SOURCES = $(wildcard $(SR)/*.c)
INCLUDES = -I./$(IN) -I./$(SR)
OBJECTS = $(SOURCES:$(SR)/%.c=$(OB)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BN)
	$(CC) $(OBJECTS) -o $@

$(OB)/%.o: $(SR)/%.c | $(OB)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OB)/* $(BN)/*

rebuild: clean all

.PHONY: all clean rebuild  
