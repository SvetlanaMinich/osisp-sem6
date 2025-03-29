#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define REQUEST_FIFO "/tmp/request_fifo" 

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <message>\n", argv[0]);
        exit(1);
    }

    char client_fifo[32];
    snprintf(client_fifo, sizeof(client_fifo), "/tmp/client_%d", getpid());
    if (mkfifo(client_fifo, 0666) == -1) {
        perror("Error creating client FIFO");
        exit(1);
    }

    char request[256];
    snprintf(request, sizeof(request), "%s:%s", client_fifo, argv[1]);

    int request_fd = open(REQUEST_FIFO, O_WRONLY);
    if (request_fd == -1) {
        perror("Error opening REQUEST_FIFO");
        exit(1);
    }
    ssize_t bytes_written = write(request_fd, request, strlen(request) + 1);
    if (bytes_written == -1) {
        perror("Error writing to request FIFO");
        close(request_fd);
        exit(1);
    }
    close(request_fd);

    int response_fd = open(client_fifo, O_RDONLY);
    if (response_fd == -1) {
        perror("Error opening client FIFO");
        exit(1);
    }
    char response[32];
    ssize_t bytes_read = read(response_fd, response, sizeof(response) - 1);
    if (bytes_read == -1) {
        perror("Error reading response");
        close(response_fd);
        exit(1);
    }
    response[bytes_read] = '\0';
    printf("Server response: %s\n", response);
    close(response_fd);

    unlink(client_fifo);

    return 0;
}