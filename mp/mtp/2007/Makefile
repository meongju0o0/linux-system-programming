CC = gcc
CFLAGS =
LDFLAGS = -lm -lpthread

.SUFFIXES: .c.o
.c.o:
	$(CC) -c $(CFLAGS) $<

ALL = calc mywc size fork thread

all: $(ALL)

calc: calc.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

mywc: mywc.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

size: size.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

fork: fork.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

thread: thread.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf *.o $(ALL)
