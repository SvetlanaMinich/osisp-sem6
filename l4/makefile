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
	./client "Test text msg"

run-clients:
	for i in 1 2 3 4; do \
		./client "Client $$i" & \
	done; \
	wait;

.PHONY: all clean run-server run-client