CFLAGS=-Wall -g

all: regressions
	:

regressions: regressions.c hash.c hash.S
	$(CC) $(CFLAGS) -o regressions regressions.c hash.c hash.S

test: regressions
	./regressions

clean:
	rm -f regressions
