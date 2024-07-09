# Makefile
CC = gcc
CFLAGS = -Wall -pthread
TARGETS = server client

all: $(TARGETS)

server: server.c database.c
	$(CC) $(CFLAGS) -o server server.c database.c -lsqlite3

client: client.c
	$(CC) $(CFLAGS) -o client client.c

clean:
	rm -f $(TARGETS)