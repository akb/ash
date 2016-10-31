.PHONY: clean

BINARY=./bin/ashi
CC=cc
CFLAGS=-std=c99 -Wall
MACOS_CFLAGS=$(CFLAGS) -ledit
LINUX_CFLAGS=$(MACOS_CFLAGS) -lm
SRC_FILES=mpc.c environment.c builtins.c node.c \
					editline.c parser.c ashi.c

$(BINARY):
	$(CC) $(CFLAGS) $(MACOS_CFLAGS) $(SRC_FILES) -o $(BINARY)

clean:
	rm -rf bin/ashi
