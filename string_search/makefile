CC=gcc
CFLAGS=-g -O0 -Wall -Werror -lm
SRCDIRS=src
INCLDIRS=include
BINDIR=bin

CFILES=$(foreach D, $(SRCDIRS), $(wildcard $(D)/*.c))
OBJFILES=$(patsubst %.c,%.o, $(CFILES))
INCL=$(foreach D, $(INCLDIRS), -I$(D))
BINARY=$(BINDIR)/main

all: $(BINARY)

$(BINARY): $(OBJFILES)
	$(CC) -o $@ $^ $(INCL) $(CFLAGS)

%.o:%.c
	$(CC) -c $< -o $@ $(INCL) $(CFLAGS)

show:
	@echo $(CFILES)
	@echo $(INCL)
	@echo $(OBJFILES)

clean:
	-rm $(BINARY) $(OBJFILES)

pkg:
	tar -zcvf string_searching.tar.gz src/ include/ bin/ makefile README tests/
