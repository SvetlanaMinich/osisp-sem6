#include <stdio.h>    // Standard I/O
#include <stdlib.h>   // Standard library (malloc, free, exit, etc.)
#include <string.h> 
#include <unistd.h>   // POSIX operating system API (read, write, fork, etc.)
#include <fcntl.h>    // File control options (open, O_RDONLY, O_WRONLY, etc.)
#include <sys/stat.h> // File status and information (mkfifo, struct stat, etc.)
#include <sys/types.h>
#include <time.h>     
#include <signal.h>   

#define REQUEST_FIFO "/tmp/request_fifo" 
#define LOG_FILE "server_log.txt"        


void log_message(const char *message) {
    FILE *log = fopen(LOG_FILE, "a");
    if (!log) {
        perror("Error opening log file");
        return;
    }
    time_t now = time(NULL);
    char *time_str = ctime(&now);
    time_str[strlen(time_str) - 1] = '\0';
    fprintf(log, "[%s] %s\n", time_str, message);
    fclose(log);
}


long long compute_expensive_task(int n) {
    long long sum = 0;
    for (int i = 1; i <= n; i++) {
        sum += (long long)i * i;
    }
    return sum;
}


void handle_request(const char *request, const char *client_fifo) {
    char log_buf[256];
    // includes buffer size limits to prevent buffer overflows
    snprintf(log_buf, sizeof(log_buf), "Start processing request from client: %s (PID: %d)", request, getpid());
    log_message(log_buf);

    long long result = compute_expensive_task(50000);
    sleep(1);

    char response[64];
    snprintf(response, sizeof(response), "Result (sum of squares): %lld", result);

    int fd = open(client_fifo, O_WRONLY);
    if (fd == -1) {
        perror("Error opening client FIFO");
        return;
    }
    ssize_t bytes_written = write(fd, response, strlen(response) + 1);
    if (bytes_written == -1) {
        perror("Error writing to client FIFO");
        close(fd);
        return;
    }
    close(fd);

    snprintf(log_buf, sizeof(log_buf), "End processing request from client: %s (PID: %d), response: %s", request, getpid(), response);
    log_message(log_buf);
}

int main() {
    // 0: Indicates octal notation
    // 6: Owner permissions (read + write = 4 + 2)
    // 6: Group permissions (read + write = 4 + 2)
    // 6: Others permissions (read + write = 4 + 2)

    // EACCES   -> Permission denied
    // EEXIST   -> File already exists
    // ENAMETOOLONG  -> Pathname too long
    // ENOENT   -> Path component doesn't exist
    // ENOSPC   -> No space left on device
    // ENOTDIR  -> Component of path not a directory

    if (mkfifo(REQUEST_FIFO, 0666) == -1) {
        perror("Error creating REQUEST_FIFO");
        exit(1);
    }

    log_message("Server started");

    while (1) {
        int request_fd = open(REQUEST_FIFO, O_RDONLY);
        if (request_fd == -1) {
            perror("Error opening REQUEST_FIFO");
            exit(1);
        }

        // Read request: format "client_fifo:message"
        char buffer[256] = {0};
        ssize_t bytes_read = read(request_fd, buffer, sizeof(buffer) - 1);
        if (bytes_read == -1) {
            perror("Error reading from REQUEST_FIFO");
            close(request_fd);
            continue;
        }
        buffer[bytes_read] = '\0';
        close(request_fd);

        if (strlen(buffer) == 0) continue;  // Empty request - skip

        char *client_fifo = strtok(buffer, ":");
        char *request = strtok(NULL, ":");
        if (!client_fifo || !request) {
            log_message("Invalid request");
            continue;
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("Fork error");
            exit(1);
        } else if (pid == 0) {
            handle_request(request, client_fifo);
            exit(0);  // Terminate child process to make parent continues running to handle new requests
        }
    }

    return 0;
}