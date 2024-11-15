CC = gcc
CFLAGS =
LDFLAGS = -lm -lpthread

.SUFFIXES: .c.o
.c.o:
	$(CC) -c $(CFLAGS) $<

ALL = conv mygrep new thread banner

all: $(ALL)

conv: conv.o
	$(CC) -o $@ $< $(LDFLAGS)

mygrep: mygrep.o
	$(CC) -o $@ $< $(LDFLAGS)

new: new.o
	$(CC) -o $@ $< $(LDFLAGS)

thread: thread.o
	$(CC) -o $@ $< $(LDFLAGS)

alarm: alarm.o
	$(CC) -o $@ $< $(LDFLAGS)

clean:
	rm -rf *.o $(ALL)
