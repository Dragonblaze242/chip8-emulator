INCLUDES = -I ./include
FLAGS = -g
OBJECTS =./build/chip8memory.o ./build/chip8stack.o ./build/chip8keyboard.o ./build/chip8.o ./build/chip8screen.o
ifeq ($(OS),Windows_NT)
    CLN = del /q build\*
	LIB = ./lib -lmingw32 -lSDL2main -lSDL2
else
	LIB = -lSDL2main -lSDL2
	CLN = rm -rf build/*
endif
all: ${OBJECTS}
	gcc ${FLAGS} ${INCLUDES} ./src/main.c ${OBJECTS} -L ${LIB} -o ./bin/main

./build/chip8memory.o:src/chip8memory.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8memory.c -c -o ./build/chip8memory.o

./build/chip8stack.o:src/chip8stack.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8stack.c -c -o ./build/chip8stack.o

./build/chip8keyboard.o:src/chip8keyboard.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8keyboard.c -c -o ./build/chip8keyboard.o

./build/chip8.o:src/chip8.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8.c -c -o ./build/chip8.o

./build/chip8screen.o:src/chip8screen.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8screen.c -c -o ./build/chip8screen.o

clean: ${CLN} 