#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    pid_t children[3];
    int i;

    printf("Parent PID: %d\n", getpid());
    printf("Creating 3 child processes...\n\n");

    /* Create multiple child processes */
    for (i = 0; i < 3; i++)
    {
        children[i] = fork();

        if (children[i] < 0)
        {
            perror("fork failed");
            exit(1);
        }

        if (children[i] == 0)
        {
            printf("Child %d: PID = %d, PPID = %d\n",
                   i + 1, getpid(), getppid());

            sleep(2 + i);

            printf("Child %d: Terminating, PID = %d\n",
                   i + 1, getpid());

            exit(0);
        }
    }

    /* Demonstrate wait() */
    printf("\nParent: Using wait()...\n");

    pid_t finished = wait(NULL);

    printf("Parent: wait() collected child PID = %d\n", finished);

    /* Demonstrate waitpid() */
    printf("\nParent: Using waitpid()...\n");

    for (i = 0; i < 2; i++)
    {
        int status;
        pid_t result = waitpid(children[i], &status, 0);

        if (result > 0)
        {
            printf("Parent: waitpid() collected child PID = %d\n",
                   result);
        }
    }

    printf("\nAll child processes have been collected.\n");

    /* Zombie process demonstration */
    printf("\n--- Zombie Process Demonstration ---\n");

    pid_t zombie = fork();

    if (zombie < 0)
    {
        perror("fork failed");
        exit(1);
    }

    if (zombie == 0)
    {
        printf("Zombie child: PID = %d\n", getpid());
        printf("Zombie child: Exiting immediately...\n");
        exit(0);
    }
    else
    {
        printf("Parent: Zombie child PID = %d\n", zombie);
        printf("Parent: Sleeping for 10 seconds without wait()...\n");
        printf("Check the process table during this time.\n");

        sleep(10);

        printf("Parent: Calling waitpid() to remove the zombie...\n");
        waitpid(zombie, NULL, 0);

        printf("Parent: Zombie process successfully reaped.\n");
    }

    printf("\nParent process terminating.\n");

    return 0;
}
