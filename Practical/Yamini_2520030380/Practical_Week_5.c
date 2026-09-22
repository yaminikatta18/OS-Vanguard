#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

#define NUM_MESSAGES 10000
#define MESSAGE_SIZE 64

double get_elapsed_time(struct timespec start, struct timespec end)
{
    return (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec) / 1000000000.0;
}

/* Producer-Consumer using Anonymous Pipe */
void producer_consumer()
{
    int pipefd[2];
    pid_t pid;
    char message[MESSAGE_SIZE];
    char buffer[256];

    long total_bytes = 0;

    if (pipe(pipefd) == -1)
    {
        perror("pipe failed");
        exit(1);
    }

    pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0)
    {
        /* Child - Consumer */
        close(pipefd[1]);

        ssize_t bytes_read;

        while ((bytes_read = read(pipefd[0], buffer,
                                  sizeof(buffer))) > 0)
        {
            total_bytes += bytes_read;
        }

        close(pipefd[0]);

        printf("Consumer (Child): Received %ld bytes\n",
               total_bytes);

        printf("Consumer (Child): Received %ld messages\n",
               total_bytes / MESSAGE_SIZE);

        printf("Consumer PID: %d\n", getpid());

        exit(0);
    }
    else
    {
        /* Parent - Producer */
        close(pipefd[0]);

        memset(message, 'A', MESSAGE_SIZE);
        message[MESSAGE_SIZE - 1] = '\n';

        struct timespec start, end;

        printf("Producer (Parent): Generating data...\n");
        printf("Producer PID: %d\n", getpid());
        printf("Number of messages: %d\n", NUM_MESSAGES);
        printf("Message size: %d bytes\n", MESSAGE_SIZE);

        clock_gettime(CLOCK_MONOTONIC, &start);

        for (int i = 0; i < NUM_MESSAGES; i++)
        {
            ssize_t total_written = 0;

            while (total_written < MESSAGE_SIZE)
            {
                ssize_t bytes_written =
                    write(pipefd[1],
                          message + total_written,
                          MESSAGE_SIZE - total_written);

                if (bytes_written <= 0)
                {
                    perror("write failed");
                    close(pipefd[1]);
                    wait(NULL);
                    exit(1);
                }

                total_written += bytes_written;
            }
        }

        close(pipefd[1]);

        wait(NULL);

        clock_gettime(CLOCK_MONOTONIC, &end);

        double elapsed =
            get_elapsed_time(start, end);

        long total_data =
            (long)NUM_MESSAGES * MESSAGE_SIZE;

        double throughput =
            (total_data / 1024.0) / elapsed;

        printf("Producer: Data sent successfully.\n");
        printf("Total data transferred: %ld bytes\n",
               total_data);

        printf("Communication time: %.6f seconds\n",
               elapsed);

        printf("Communication throughput: %.2f KB/s\n",
               throughput);

        printf("Producer: Consumer process completed.\n");
    }
}

/* Equivalent of: ls -l | grep ".c" */
void pipeline_demo()
{
    int pipefd[2];
    pid_t ls_pid;
    pid_t grep_pid;

    printf("\n--- ls -l | grep \".c\" Demonstration ---\n");

    if (pipe(pipefd) == -1)
    {
        perror("pipe failed");
        exit(1);
    }

    /* First child executes ls -l */
    ls_pid = fork();

    if (ls_pid < 0)
    {
        perror("fork failed");
        exit(1);
    }

    if (ls_pid == 0)
    {
        dup2(pipefd[1], STDOUT_FILENO);

        close(pipefd[0]);
        close(pipefd[1]);

        execlp("ls", "ls", "-l", (char *)NULL);

        perror("exec ls failed");
        exit(1);
    }

    /* Second child executes grep ".c" */
    grep_pid = fork();

    if (grep_pid < 0)
    {
        perror("fork failed");
        exit(1);
    }

    if (grep_pid == 0)
    {
        dup2(pipefd[0], STDIN_FILENO);

        close(pipefd[0]);
        close(pipefd[1]);

        execlp("grep", "grep", ".c", (char *)NULL);

        perror("exec grep failed");
        exit(1);
    }

    /* Parent closes pipe and waits */
    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(ls_pid, NULL, 0);
    waitpid(grep_pid, NULL, 0);

    printf("Pipeline execution completed successfully.\n");
}

int main()
{
    printf("========================================\n");
    printf("       OS PRACTICAL - WEEK 5\n");
    printf("========================================\n");

    printf("\n--- Producer-Consumer using Anonymous Pipe ---\n");

    producer_consumer();

    pipeline_demo();

    printf("\nAll Week 5 operations completed successfully.\n");

    return 0;
}
