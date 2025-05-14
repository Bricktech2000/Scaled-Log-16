CC=gcc
CFLAGS=-O2 -Wall -Wextra -Wpedantic -std=c99

all: bin/linear bin/quadratic

bin/linear: compare.c sl16.c sl16.h | bin/
	$(CC) $(CFLAGS) -O3 -Wno-sign-compare -Wno-parentheses \
		-DSL16_LINEAR compare.c sl16.c -lm -o $@

bin/quadratic: compare.c sl16.c sl16.h | bin/
	$(CC) $(CFLAGS) -O3 -Wno-sign-compare -Wno-parentheses \
		-DSL16_QUADRATIC compare.c sl16.c -lm -o $@

bin/:
	mkdir bin/

clean:
	rm -rf bin/
