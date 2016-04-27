CC=gcc
CFLAGS=-O3 -march=native
DEPS=determinant.h
OBJ=det.o determinant.o

all: det

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

det: det.o determinant.o
	$(CC) $(OBJ) -o det -lm

clean:
	rm det $(OBJ)
