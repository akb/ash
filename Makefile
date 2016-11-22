.PHONY: all clean test

BINARY=./bin/ash
CC=cc
CFLAGS=-std=c99 -Wall
MACOS_CFLAGS=$(CFLAGS) -ledit
LINUX_CFLAGS=$(MACOS_CFLAGS) -lm
SRC_FILES=mpc.c environment.c builtins.c node.c \
					editline.c parser.c ash.c debug.c

all: clean $(BINARY) test

$(BINARY):
	$(CC) $(CFLAGS) $(MACOS_CFLAGS) $(SRC_FILES) -o $(BINARY)

linux:
	$(CC) $(CFLAGS) $(LINUX_CFLAGS) $(SRC_FILES) -o $(BINARY)

clean:
	rm -rf bin/ash

test:
	csi -s spec/ash.spec.scm