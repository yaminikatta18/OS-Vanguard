#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define REQUEST_FIFO "/tmp/yamini_request_fifo"
#define RESPONSE_FIFO "/tmp/yamini_response_fifo"

int main()
{
    char message[200];
    char response[200];

    printf("========================================\n");
    printf("       WEEK 6 - FIFO CLIENT\n");
    printf("========================================\n");

    printf("Enter message for server: ");
    fgets(message, sizeof(message), stdin);

    message[strcspn(message, "\n")] = '\0';

    /* Open request FIFO for writing */
    int request_fd = open(REQUEST_FIFO, O_WRONLY);

    if (request_fd == -1)
    {
        perror("Error opening request FIFO");
        return 1;
    }

    /* Send message to server */
    write(request_fd, message, strlen(message) + 1);

    printf("Client: Message sent to server.\n");

    close(request_fd);

    /* Open response FIFO for reading */
    int response_fd = open(RESPONSE_FIFO, O_RDONLY);

    if (response_fd == -1)
    {
        perror("Error opening response FIFO");
        return 1;
    }

    /* Receive server response */
    ssize_t bytes_read =
        read(response_fd, response, sizeof(response) - 1);

    if (bytes_read > 0)
    {
        response[bytes_read] = '\0';

        printf("Client: Received -> %s\n", response);
    }

    close(response_fd);

    printf("Client: FIFO communication completed.\n");

    return 0;
}
