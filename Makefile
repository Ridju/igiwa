##
#
#
# @file
# @version 0.1
CC = gcc
CFLAGS = -W -Wall -Wextra -pedantic
TMP = `sdl2-config --libs --cflags` -lSDL2
BUILD_DIR = build
SDL_FLAGS = -Iinclude -lpthread -Llib -lSDL2 -lSDL2main

chip8: main.o display.o utils.o cpu.o keyboard.o
	$(CC) $(CFLAGS) $(BUILD_DIR)/main.o $(BUILD_DIR)/display.o $(BUILD_DIR)/utils.o $(BUILD_DIR)/cpu.o $(BUILD_DIR)/keyboard.o -o $(BUILD_DIR)/chip8 $(SDL_FLAGS)

main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o $(BUILD_DIR)/main.o

display.o: src/display/display.c
	$(CC) $(CFLAGS) -c src/display/display.c -o $(BUILD_DIR)/display.o 

utils.o: src/utils/utils.c
	$(CC) $(CFLAGS) -c src/utils/utils.c -o $(BUILD_DIR)/utils.o 

cpu.o: src/cpu/cpu.c
	$(CC) $(CFLAGS) -c src/cpu/cpu.c -o $(BUILD_DIR)/cpu.o 

keyboard.o: src/keyboard/keyboard.c
	$(CC) $(CFLAGS) -c src/keyboard/keyboard.c -o $(BUILD_DIR)/keyboard.o 

clean:
	rm -r $(BUILD_DIR)/*.o $(BUILD_DIR)/chip8

run:
	cd ./build && ./vm && cd ..
# end
