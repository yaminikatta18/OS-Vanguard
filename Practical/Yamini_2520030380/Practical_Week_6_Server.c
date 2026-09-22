#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define REQUEST_FIFO "/tmp/yamini_request_fifo"
#define RESPONSE_FIFO "/tmp/yamini_response_fifo"

int main()
{
    char message[200];
    char response[200];

    printf("========================================\n");
    printf("       WEEK 6 - FIFO SERVER\n");
    printf("========================================\n");

    /* Create named pipes */
    mkfifo(REQUEST_FIFO, 0666);
    mkfifo(RESPONSE_FIFO, 0666);

    printf("Server PID: %d\n", getpid());
    printf("Waiting for client message...\n\n");

    /* Open request FIFO for reading */
    int request_fd = open(REQUEST_FIFO, O_RDONLY);

    if (request_fd == -1)
    {
        perror("Error opening request FIFO");
        return 1;
    }

    /* Read message from client */
    ssize_t bytes_read = read(request_fd, message, sizeof(message) - 1);

    if (bytes_read > 0)
    {
        message[bytes_read] = '\0';

        printf("Server: Message received -> %s\n", message);

        /* Process message */
        snprintf(response, sizeof(response),
                 "Server response: Message processed successfully.");

        close(request_fd);

        /* Open response FIFO for writing */
        int response_fd = open(RESPONSE_FIFO, O_WRONLY);

        if (response_fd == -1)
        {
            perror("Error opening response FIFO");
            return 1;
        }

        write(response_fd, response, strlen(response) + 1);

        printf("Server: Response sent to client.\n");

        close(response_fd);
    }

    /* Remove named pipes */
    unlink(REQUEST_FIFO);
    unlink(RESPONSE_FIFO);

    printf("\nServer: FIFO communication completed.\n");

    return 0;
}
