EXE=gtk_tetris

CC=g++

SRC=src/main.cc

CFLAGS+=-Isrc/ `pkg-config --cflags gtk+-3.0 gl glew`
LDFLAGS+=-Ilib/ `pkg-config --libs gtk+-3.0 gl glew`

all:
	${CC} ${CFLAGS} -o ${EXE} ${SRC} ${LDFLAGS}
