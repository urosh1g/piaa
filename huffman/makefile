CC=gcc
CFLAGS=-pedantic -Werror -Iinclude -O3

all: main generator

main: main.o
	$(CC) -o ./bin/main main.o $(CFLAGS)

generator: generator.o
	$(CC) -o ./bin/input_generator generator.o  $(CFLAGS)

main.o:
	$(CC) -c src/main.c -o main.o $(CFLAGS)

generator.o:
	$(CC) -c src/input_generator.c -o generator.o $(CFLAGS)

.PHONY: clean

clean:
	-rm main.o generator.o ./bin/main ./bin/input_generator
