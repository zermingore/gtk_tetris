EXE=gtk_tetris

CC=g++

SRC=src/main.cc src/Shaders.cc src/Grid.cc

CFLAGS+=-Wall -Wextra -Isrc/ -Ilibs/ `pkg-config --cflags gtk+-3.0 gl glew`
LDFLAGS+=-Ilibs/ `pkg-config --libs gtk+-3.0 gl glew`

all:
	${CC} ${CFLAGS} -o ${EXE} ${SRC} ${LDFLAGS}
