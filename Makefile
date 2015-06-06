CFLAGS=-Wall -Wextra -pedantic -g `pkg-config --cflags sdl2`
LDFLAGS=`pkg-config --libs sdl2` -lm -lnetpbm
CC=gcc -std=gnu99
OBJ=main.o audio.o image.o options.o image_audio.o common.o
NAME=hearimg

all: $(OBJ)
	$(CC) $(LDFLAGS) -o $(NAME) $^

main.o: src/main.c src/image_audio.h src/options.h
	$(CC) -c $(CFLAGS) $^

audio.o: src/audio.c src/audio.h
	$(CC) -c $(CFLAGS) $^

image.o: src/image.c src/image.h
	$(CC) -c $(CFLAGS) $^

options.o: src/options.c src/options.h
	$(CC) -c $(CFLAGS) $^

image_audio.o: src/image_audio.c src/image_audio.h src/image.h src/audio.h
	$(CC) -c $(CFLAGS) $^

common.o: src/common.c src/common.h
	$(CC) -c $(CFLAGS) $^

clean:
	rm -f $(NAME) $(OBJ) src/*.h.gch
