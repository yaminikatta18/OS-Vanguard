#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_PATHS 128

void demonstrate_waitpid()
{
    printf("\n===============================================\n");
    printf("       PROCESS SYNCHRONIZATION - waitpid()\n");
    printf("===============================================\n");

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        printf("Child process started.\n");
        printf("Child PID  : %d\n", getpid());
        printf("Parent PID : %d\n", getppid());

        sleep(2);

        printf("Child process completed.\n");
        exit(0);
    }
    else
    {
        int status;
        printf("Parent PID : %d\n", getpid());
        printf("Waiting for child process using waitpid()...\n");

        pid_t result = waitpid(pid, &status, 0);

        if (result == pid)
        {
            if (WIFEXITED(status))
            {
                printf("Child PID %d terminated normally.\n", pid);
                printf("Exit status : %d\n", WEXITSTATUS(status));
            }
        }
    }
}

void resolve_command(const char *command)
{
    char *path_env = getenv("PATH");

    if (path_env == NULL)
    {
        printf("PATH variable not found.\n");
        return;
    }

    char path_copy[4096];
    strncpy(path_copy, path_env, sizeof(path_copy) - 1);
    path_copy[sizeof(path_copy) - 1] = '\0';

    char *directory = strtok(path_copy, ":");

    while (directory != NULL)
    {
        char full_path[1024];

        snprintf(full_path, sizeof(full_path),
                 "%s/%s", directory, command);

        if (access(full_path, X_OK) == 0)
        {
            printf("Executable found : %s\n", full_path);
            printf("Execute permission : Available\n");
            return;
        }

        directory = strtok(NULL, ":");
    }

    printf("Command '%s' not found in PATH.\n", command);
}

void demonstrate_path()
{
    printf("\n===============================================\n");
    printf("          PATH VARIABLE RESOLUTION\n");
    printf("===============================================\n");

    char *path = getenv("PATH");

    if (path == NULL)
    {
        printf("PATH variable is not available.\n");
        return;
    }

    printf("PATH variable:\n%s\n", path);

    printf("\nSearching for executable commands...\n");

    printf("\nCommand: ls\n");
    resolve_command("ls");

    printf("\nCommand: gcc\n");
    resolve_command("gcc");

    printf("\nCommand: nonexistent_command\n");
    resolve_command("nonexistent_command");
}

int main()
{
    printf("===============================================\n");
    printf("          SKILL WEEK 7 - PROCESS & PATH\n");
    printf("===============================================\n");

    demonstrate_waitpid();
    demonstrate_path();

    printf("\n===============================================\n");
    printf("       SKILL WEEK 7 COMPLETED SUCCESSFULLY\n");
    printf("===============================================\n");

    return 0;
}
