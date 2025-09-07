CC = clang
SRC = main.c
INCLUDE = -I/opt/homebrew/include/SDL2
LIB = -L/opt/homebrew/lib -lSDL2

all:
	$(CC) $(SRC) $(INCLUDE) $(LIB)
