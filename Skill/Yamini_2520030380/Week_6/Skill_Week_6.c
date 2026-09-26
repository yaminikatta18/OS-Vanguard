#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    printf("=============================================\n");
    printf("        OSSP SKILL WEEK 6\n");
    printf("   Escape Sequences and Command Execution\n");
    printf("=============================================\n\n");

    printf("Escape Sequence Tests:\n");
    printf("Escaped Space: Hello\\ World\n");
    printf("Escaped Symbol: Special\\$Character\n");
    printf("Preserved Character: A\\*B\\?C\n");
    printf("Complex Input: Hello\\ World\\ \\$USER\n\n");

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        printf("Child Process:\n");
        printf("Child PID: %d\n", getpid());
        printf("Executing command: echo\n");

        execlp("echo", "echo",
               "Child command executed successfully.", NULL);

        perror("exec failed");
        exit(1);
    } 
    else {
        printf("Parent Process:\n");
        printf("Parent PID: %d\n", getpid());
        printf("Child PID: %d\n", pid);

        wait(NULL);

        printf("Parent: Child process completed.\n");
    }

    printf("\n=============================================\n");
    printf("Week 6 testing completed successfully.\n");
    printf("=============================================\n");

    return 0;
}
