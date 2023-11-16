CC=gcc
CFLAGS=-O3 -Iinclude -Wall -Wextra
SRCDIR=src
BINDIR=bin
DEPS=sorting.o main.o

CFILES=$(foreach D, $(SRCDIR), $(wildcard $(D)/*.c))
OBJ=$(patsubst %.c, %.o, $(CFILES))
BINARY=main

all: $(BINARY)

$(BINARY): $(OBJ)
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS)

%.o:%.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	-rm $(OBJ) $(BINDIR)/$(BINARY)
