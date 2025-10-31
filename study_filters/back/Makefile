CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -g

SR = src
IN = inc
OB = obj
BN = bin

TARGET = $(BN)/filter

SOURCES = $(wildcard $(SR)/*.c)
INCLUDES = -I./$(IN) -I./$(SR)
OBJECTS = $(SOURCES:$(SR)/%.c=$(OB)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BN)
	$(CC) $(OBJECTS) -o $@ -lm

$(OB)/%.o: $(SR)/%.c | $(OB)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OB)/* $(BN)/*

.PHONY: all

