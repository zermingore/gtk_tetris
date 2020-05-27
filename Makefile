EXE=gtk_tetris

CC=g++

SRC=main.c

CFLAGS+=`pkg-config --cflags gtk+-3.0 gl glew`
LDFLAGS+=`pkg-config --libs gtk+-3.0 gl glew`

all:
	${CC} ${CFLAGS} -o ${EXE} ${SRC} ${LDFLAGS}
