#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void producer_consumer()
{
    int pipefd[2];
    pid_t pid;
    char buffer[100];
    int data = 100;

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

        read(pipefd[0], buffer, sizeof(buffer));

        printf("Consumer (Child): Received data -> %s\n", buffer);
        printf("Consumer PID: %d\n", getpid());

        close(pipefd[0]);
        exit(0);
    }
    else
    {
        /* Parent - Producer */
        close(pipefd[0]);

        printf("Producer (Parent): Generating data...\n");

        snprintf(buffer, sizeof(buffer),
                 "Data value = %d", data);

        clock_t start = clock();

        write(pipefd[1], buffer, strlen(buffer) + 1);

        clock_t end = clock();

        double time_taken =
            (double)(end - start) / CLOCKS_PER_SEC;

        printf("Producer PID: %d\n", getpid());
        printf("Producer: Data sent through pipe.\n");
        printf("Communication time: %.6f seconds\n",
               time_taken);

        close(pipefd[1]);

        wait(NULL);

        printf("Producer: Consumer process completed.\n");
    }
}

void pipeline_demo()
{
    int pipefd[2];
    pid_t ls_pid, grep_pid;

    printf("\n--- ls -l | grep \".c\" Demonstration ---\n");

    if (pipe(pipefd) == -1)
    {
        perror("pipe failed");
        exit(1);
    }

    /* First child: ls -l */
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

    /* Second child: grep ".c" */
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

    /* Parent */
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
