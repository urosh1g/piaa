CC=gcc
CFLAGS=-Wall -Wextra -pedantic -O0 -lm
SRCDIRS=src
INCLDIRS=include
BINDIR=bin
BINARY=$(BINDIR)/main

CFILES=$(foreach D, $(SRCDIRS), $(wildcard $(D)/*.c))
DEPS=$(foreach D, $(INCLDIRS), $(wildcard $(D)/*.h))
OBJS=$(patsubst %.c,%.o, $(CFILES))
INCLUDE=$(foreach D, $(INCLDIRS),-I$(D))

all: $(BINARY)

$(BINARY): $(OBJS)
	$(CC) $^ -o $@ $(CFLAGS) $(INCLUDE)

%.o:%.c $(DEPS)
	$(CC) -c $< $(CFLAGS) $(INCLUDE) -o $@

pkg:
	zip dijkstra.zip $(SRCDIRS) $(INCLDIRS) $(BINDIR) makefile README -r

show:
	@echo $(CFILES)
	@echo $(DEPS)
	@echo $(OBJS)
	@echo $(INCLUDE)

clean:
	-rm $(OBJS) $(BINARY) dijkstra.zip
