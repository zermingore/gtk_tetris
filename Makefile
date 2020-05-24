EXE=gtk_tetris
SRC=main.c

all:
	gcc `pkg-config --cflags gtk+-3.0 gl glew` -o ${EXE} ${SRC} `pkg-config --libs gtk+-3.0 gl glew`
