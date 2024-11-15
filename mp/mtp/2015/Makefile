CC = gcc
CFLAGS =
LDFLAGS = -lm -lpthread

SUFFIXES = .c.o
.c.o:
	$(CC) -c $(CFLAGS) $<

ALL = power mystrrchr calc mymv2 fork

all: $(ALL)

power: power.o
	$(CC) -o $@ $< $(LDFLAGS)

mystrrchr: mystrrchr.o
	$(CC) -o $@ $< $(LDFLAGS)

calc: calc.o
	$(CC) -o $@ $< $(LDFLAGS)

mymv2: mymv2.o
	$(CC) -o $@ $< $(LDFLAGS)

fork: fork.o
	$(CC) -o $@ $< $(LDFLAGS)

clean:
	rm -rf *.o $(ALL)
