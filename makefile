CC=gcc
LD=gcc
CFLAGS=--std=c99 --pedantic -Wall -W -Wmissing-prototypes -DNDEBUG
LDFLAGS=-lm

all: main tries dictionary

.c .o:
	$(CC) -c $(CFLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -f *.o

test: test_is_word_in_trie.o tries.o dictionary.o
	$(LD) -o $@ $^ $(LDFLAGS)

solver: main.o grid.o dictionary.o tries.o
	$(LD) -o $@ $^ $(LDFLAGS)
	
tries: tries.o
	$(LD) -o $@ $^ $(LDFLAGS)

dictionary: dictionary.o tries.o
	$(LD) -o $@ $^ $(LDFLAGS)

grid: grid.o tries.o
	$(LD) -o $@ $^ $(LDFLAGS)