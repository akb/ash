.PHONY: clean

BINARY=./bin/ashi
CC=cc
CFLAGS=-std=c99 -Wall
MACOS_CFLAGS=$(CFLAGS) -ledit
LINUX_CFLAGS=$(MACOS_CFLAGS) -lm
SRC_FILES=ashi.c mpc.c result.c eval.c editline.c

$(BINARY):
	$(CC) $(CFLAGS) $(MACOS_CFLAGS) $(SRC_FILES) -o $(BINARY)

clean:
	rm -rf bin/ashi
