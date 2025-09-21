CC = clang
SRC = main.c

SDL_INCLUDE = /opt/homebrew/include/
SDL_LIB = /opt/homebrew/lib

DAROOT = /users/nicotsiridis/daroot
HII_PATH = $(DAROOT)/github~nctsiridis/hii@v1.0.0
BIN = program

CFLAGS = -I$(SDL_INCLUDE) -I$(HII_PATH)
LDFLAGS = -L$(SDL_LIB) -lsdl2 -lSDL2_ttf

all: build_darman build_core build_modules build/hash.o build/canvas.o build/core.o
	$(CC) $(SRC) $(CFLAGS) $(LDFLAGS) build/core.o build/canvas.o build/hash.o -o $(BIN)

build_darman:
	mkdir -p build
	$(CC) $(CFLAGS) -c $(HII_PATH)/hash.c -o build/hash.o

build_core:
	mkdir -p build
	$(CC) $(CFLAGS) -c core.c -o build/core.o

build_modules:
	mkdir -p build
	$(CC) $(CFLAGS) -c context_modules/canvas.c -o build/canvas.o

run: program
	./program
