CC = gcc
CFLAGS =
LDFLAGS = -lm -lpthread

.SUFFIXES: .c.o
.c.o:
	$(CC) -c $(CFLAGS) $<

ALL = sum str upper exec thread

all: $(ALL)

sum: sum.o
	$(CC) -o $@ $< $(LDFLAGS)

str: str.o
	$(CC) -o $@ $< $(LDFLAGS)

upper: upper.o
	$(CC) -o $@ $< $(LDFLAGS)

exec: exec.o
	$(CC) -o $@ $< $(LDFLAGS)

thread: thread.o
	$(CC) -o $@ $< $(LDFLAGS)

clean:
	rm -rf *.o $(ALL)
