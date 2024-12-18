CC = gcc
CFLAGS =
LDFLAGS = -lm -lpthread

SUFFIXES = .c.o
.c.o:
	$(CC) -c $(CFLAGS) $<

ALL = rand msngr server client

all : $(ALL)

rand: rand.o
	$(CC) -o $@ $< $(LDFLAGS)

msngr: msngr.o
	$(CC) -o $@ $< $(LDFLAGS)

server: server.o
	$(CC) -o $@ $< $(LDFLAGS)

client: client.o
	$(CC) -o $@ $< $(LDFLAGS)

clean:
	rm -rf *.o $(ALL)
