CC = gcc
#CFLAGS = -g -Wall -Wextra -Werror -pedantic
CFLAGS = -g -fsanitize=address -Wall -Wextra -Werror -pedantic
LDFLGS = -fsanitize=address -Wl,-Map=output.map,--cref
#, в случае, если нужно проверить память

SR = src
IN = inc
OB = obj
BN = bin

TARGET = $(BN)/dm

SOURCES = $(wildcard $(SR)/*.c)
INCLUDES = -I./$(IN) -I./$(SR)
OBJECTS = $(SOURCES:$(SR)/%.c=$(OB)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BN)
	$(CC) $(LDFLGS) $(OBJECTS) -o $@ -lm -lpthread 
#-lasan, в случае, если нужно проверить память

$(OB)/%.o: $(SR)/%.c | $(OB)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OB)/* $(BN)/*
	rm *.map

rebuild: clean all

.PHONY: all clean rebuild  