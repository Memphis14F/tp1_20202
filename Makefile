CFLAGS=-Wall -g

all: tp1
	:

tp1: main.c hash.c hash.S parser.c
	$(CC) $(CFLAGS) -o tp1 main.c hash.c hash.S parser.c

clean:
	rm -f regressions