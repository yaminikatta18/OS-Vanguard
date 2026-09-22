#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t signal_count = 0;

void handle_signal(int sig)
{
    if (sig == SIGINT)
        printf("\nSIGINT received! Ctrl+C signal handled.\n");
    else if (sig == SIGTERM)
        printf("SIGTERM received! Termination signal handled.\n");
    else if (sig == SIGUSR1)
        printf("SIGUSR1 received! User-defined signal handled.\n");

    fflush(stdout);
    signal_count++;
}

int main()
{
    printf("========================================\n");
    printf("     WEEK 6 - POSIX SIGNAL HANDLING\n");
    printf("========================================\n");

    printf("Process ID (PID): %d\n", getpid());
    printf("Waiting for SIGINT, SIGTERM and SIGUSR1...\n\n");

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    signal(SIGUSR1, handle_signal);

    while (signal_count < 3)
    {
        pause();
    }

    printf("\nAll three signals were handled successfully.\n");
    printf("Signal handling demonstration completed.\n");

    return 0;
}

