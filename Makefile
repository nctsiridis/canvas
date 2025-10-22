CC = clang
SRC = main.c

SDL_INCLUDE = /opt/homebrew/include/
SDL_LIB = /opt/homebrew/lib

DAROOT = /users/nicotsiridis/daroot
HII_PATH = $(DAROOT)/github~nctsiridis/hii@v1.0.0
HASH_PATH = $(DAROOT)/github~nctsiridis/hash@v1.0.0
CJSON_PATH = $(DAROOT)/github~DaveGamble/cJSON@v1.7.19
FILEUTILS_PATH = $(DAROOT)/github~barrust/c-utils@v0.2.5/src
BIN = build/program

DFLAGS = -Wall -Wextra
CFLAGS = -I$(SDL_INCLUDE) -I$(HII_PATH) -I$(HASH_PATH) -I$(CJSON_PATH) -I$(FILEUTILS_PATH)
LDFLAGS = -L$(SDL_LIB) -lsdl2 -lSDL2_ttf

all: build_darman build_core build_modules build/hash.o build/canvas.o build/core.o
	$(CC) $(DFLAGS) $(SRC) $(CFLAGS) $(LDFLAGS) \
	build/core.o build/canvas.o build/hash.o build/map_unordered.o build/cJSON.o build/fileutils.o -o $(BIN)

build_darman:
	mkdir -p build
	$(CC) $(CFLAGS) -c $(HII_PATH)/hash.c -o build/hash.o
	$(CC) $(CFLAGS) -c $(HASH_PATH)/map_unordered.c -o build/map_unordered.o
	$(CC) $(CFLAGS) -c $(CJSON_PATH)/cJSON.c -o build/cJSON.o
	$(CC) $(CFLAGS) -c $(FILEUTILS_PATH)/fileutils.c -o build/fileutils.o

build_core:
	mkdir -p build
	$(CC) $(CFLAGS) -c core.c -o build/core.o

build_modules:
	mkdir -p build
	$(CC) $(CFLAGS) -c context_modules/canvas.c -o build/canvas.o

run: all
	./build/program

debug: all
	lldb ./build/program
