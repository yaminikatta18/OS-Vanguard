#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int *data = malloc(5 * sizeof(int));

    if (data == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < 5; i++)
    {
        data[i] = (i + 1) * 10;
    }

    printf("===============================================\n");
    printf("       WEEK 8 - COPY-ON-WRITE\n");
    printf("===============================================\n");

    printf("Parent PID: %d\n", getpid());

    printf("\nBefore fork():\n");
    printf("Data address : %p\n", (void *)data);
    printf("Data values  : ");

    for (int i = 0; i < 5; i++)
    {
        printf("%d ", data[i]);
    }

    printf("\n");

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        free(data);
        return 1;
    }

    if (pid == 0)
    {
        printf("\n--- Child Process ---\n");
        printf("Child PID: %d\n", getpid());
        printf("Data address before modification: %p\n", (void *)data);

        printf("Child modifying data...\n");
        data[0] = 999;

        printf("Data address after modification : %p\n", (void *)data);
        printf("Child data values: ");

        for (int i = 0; i < 5; i++)
        {
            printf("%d ", data[i]);
        }

        printf("\n");
        printf("Copy-on-Write: Child gets its own modified memory page.\n");

        free(data);
        exit(0);
    }
    else
    {
        wait(NULL);

        printf("\n--- Parent Process ---\n");
        printf("Parent PID: %d\n", getpid());
        printf("Parent data address: %p\n", (void *)data);

        printf("Parent data values: ");

        for (int i = 0; i < 5; i++)
        {
            printf("%d ", data[i]);
        }

        printf("\n");
        printf("Parent data remains unchanged after child modification.\n");
    }

    free(data);

    printf("\n===============================================\n");

    return 0;
}
