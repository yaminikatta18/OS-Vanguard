#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

void print_process_info(const char *stage)
{
    printf("%s\n", stage);
    printf("PID  : %d\n", getpid());
    printf("PPID : %d\n", getppid());
    fflush(stdout);
}

int main()
{
    pid_t pid;

    print_process_info("Parent process - Before fork");

    pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }

    if (pid == 0)
    {
        print_process_info("Child process - Running");

        printf("Child process: Going to sleep (Waiting state)...\n");
        fflush(stdout);

        sleep(10);

        print_process_info("Child process - Running again");

        printf("Child process - Terminating\n");
        return 0;
    }
    else
    {
        print_process_info("Parent process - Running");

        printf("Parent process: Waiting for child to finish...\n");
        fflush(stdout);

        wait(NULL);

        print_process_info("Parent process - Child terminated");
        printf("Parent process - Terminating\n");
    }

    return 0;
}
