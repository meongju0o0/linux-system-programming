CC = gcc
CFLAGS =
LDFLAGS = -lm -lpthread

.SUFFIXES: .c.o
.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

ALL = sig sync ftps/ftps ftpc/ftpc

all: $(ALL)

sig: sig.o
	$(CC) -o $@ $< $(LDFLAGS)

sync: sync.o
	$(CC) -o $@ $< $(LDFLAGS)

ftps/ftps: ftps/ftps.o
	$(CC) -o $@ $< $(LDFLAGS)

ftpc/ftpc: ftpc/ftpc.o
	$(CC) -o $@ $< $(LDFLAGS)

clean:
	rm -rf *.o ftps/*.o ftpc/*.o $(ALL)
