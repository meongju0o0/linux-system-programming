CC = gcc
CFLAGS =
LDFLAGS = -lpthread

.SUFFIXES: .c.o
.c.o:
	$(CC) -c $(CFLAGS) $<

ALL = order tcps tcpc udps udpc tcpc_dns myusleep select tcps_p select_t

all: $(ALL)

order: order.o
	$(CC) -o $@ $< $(LDFLAGS)

tcps: tcps.o
	$(CC) -o $@ $< $(LDFLAGS)

tcpc: tcpc.o
	$(CC) -o $@ $< $(LDFLAGS)

udps: udps.o
	$(CC) -o $@ $< $(LDFLAGS)

udpc: udpc.o
	$(CC) -o $@ $< $(LDFLAGS)

tcpc_dns: tcpc_dns.o
	$(CC) -o $@ $< $(LDFLAGS)

myusleep: myusleep.o
	$(CC) -o $@ $< $(LDFLAGS)

select: select.o
	$(CC) -o $@ $< $(LDFLAGS)

tcps_p: tcps_p.o
	$(CC) -o $@ $< $(LDFLAGS)

select_t: select_t.o
	$(CC) -o $@ $< $(LDFLAGS)

clean:
	rm -rf *.o $(ALL)
