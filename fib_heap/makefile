CC=gcc
CFLAGS=-Wall -Wextra -g -O0 -pedantic
INCLDIRS=include
LINKS=-lm
SRCDIRS=src
BINARY=bin/fib_heap

DEPS=$(foreach D, $(INCLDIRS), $(wildcard $(D)/*.h))
CFILES=$(foreach D, $(SRCDIRS), $(wildcard $(D)/*.c))
OBJFILES=$(patsubst %.c, %.o, $(CFILES))
INCL=$(foreach D, $(INCLDIRS),-I$(D))

all: $(BINARY) 
	
$(BINARY): $(OBJFILES)
	$(CC) $(CFLAGS) $(INCL) $^ -o $@ $(LINKS)

%.o:%.c $(DEPS)
	$(CC) $(CFLAGS) $(INCL) -c $< -o $@ $(LINKS)

pkg:
	zip fib_heap.zip $(SRCDIRS) $(INCLDIRS) bin/ makefile README make_gif.sh -r

show:
	@echo $(DEPS)
	@echo $(CFILES)
	@echo $(OBJFILES)
	@echo $(CC) $(CFLAGS) $(INCL) -o $(EXE)

clean:
	-rm $(BINARY) $(OBJFILES) $(wildcard *.png) $(wildcard *.dot) $(wildcard *.gif)
