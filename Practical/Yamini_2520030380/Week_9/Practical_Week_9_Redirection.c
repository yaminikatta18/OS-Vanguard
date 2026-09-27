#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int input_fd, output_fd;

    printf("===============================================\n");
    printf("       WEEK 9 - I/O REDIRECTION USING dup2()\n");
    printf("===============================================\n\n");

    /* Open input file */
    input_fd = open("input.txt", O_RDONLY);

    if (input_fd < 0)
    {
        perror("open input.txt");
        return 1;
    }

    /* Redirect standard input to input.txt */
    if (dup2(input_fd, STDIN_FILENO) < 0)
    {
        perror("dup2 input");
        close(input_fd);
        return 1;
    }

    close(input_fd);

    /* Open output file */
    output_fd = open("output.txt",
                     O_WRONLY | O_CREAT | O_TRUNC,
                     0644);

    if (output_fd < 0)
    {
        perror("open output.txt");
        return 1;
    }

    /* Redirect standard output to output.txt */
    if (dup2(output_fd, STDOUT_FILENO) < 0)
    {
        perror("dup2 output");
        close(output_fd);
        return 1;
    }

    close(output_fd);

    printf("This message was redirected to output.txt\n");
    printf("Standard input is redirected from input.txt\n");
    printf("Standard output is redirected to output.txt\n");
    printf("dup2() successfully demonstrated I/O redirection.\n");

    return 0;
}
