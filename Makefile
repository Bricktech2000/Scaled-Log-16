CC=gcc
CFLAGS=-O2 -Wall -Wextra -Wpedantic -std=c99

all: bin/test

bin/test: test.c sl16.c sl16.h | bin/
	$(CC) $(CFLAGS) -Wno-sign-compare -Wno-parentheses \
		test.c sl16.c -lm -o $@

bin/:
	mkdir bin/

clean:
	rm -rf bin/
