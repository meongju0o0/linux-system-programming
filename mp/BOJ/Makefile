# Makefile for BOJ(Baekjoon Online Judge) Build & Debug

CC = gcc
CFLAGS = -Wall -O2 -Wno-unused-result
LDFLAGS = -L../lib -lm
TARGET = BOJ6502
SRCS = BOJ6502.c

BUILD_OBJS = $(addprefix build/, $(SRCS:.c=.o))
DEBUG_OBJS = $(addprefix debug/, $(SRCS:.c=.o))

.SUFFIXES = .c.o

all: build debug

build/%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

debug/%.o: %.c
	$(CC) -c $(CFLAGS) -g -o $@ $<

build: $(BUILD_OBJS)
	$(CC) -o build/$(TARGET) $(BUILD_OBJS) $(LDFLAGS)

debug: $(DEBUG_OBJS)
	$(CC) -g -o debug/$(TARGET) $(DEBUG_OBJS) $(LDFLAGS)

clean:
	rm -rf build debug
	mkdir -p build debug
