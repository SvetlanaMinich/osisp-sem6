CC = gcc
CFLAGS = -Wall -g -O2
TARGETS = server client

all: $(TARGETS)

server: server.c
	$(CC) $(CFLAGS) -o server server.c

client: client.c
	$(CC) $(CFLAGS) -o client client.c

clean:
	rm -f $(TARGETS) *.o /tmp/client_* /tmp/request_fifo server_log.txt

run-server:
	./server &

run-client:
	./client "Text msg"

.PHONY: all clean run-server run-client