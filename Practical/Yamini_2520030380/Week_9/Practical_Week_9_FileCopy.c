#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define BUFFER_SIZE 1024

void low_level_copy(const char *source, const char *destination)
{
    int src = open(source, O_RDONLY);
    int dest = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (src < 0 || dest < 0)
    {
        perror("open");
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes;

    while ((bytes = read(src, buffer, BUFFER_SIZE)) > 0)
    {
        write(dest, buffer, bytes);
    }

    /* Demonstrate lseek() */
    lseek(src, 0, SEEK_SET);

    close(src);
    close(dest);
}

void standard_copy(const char *source, const char *destination)
{
    FILE *src = fopen(source, "r");
    FILE *dest = fopen(destination, "w");

    if (src == NULL || dest == NULL)
    {
        perror("fopen");
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    size_t bytes;

    while ((bytes = fread(buffer, 1, BUFFER_SIZE, src)) > 0)
    {
        fwrite(buffer, 1, bytes, dest);
    }

    fclose(src);
    fclose(dest);
}

int main()
{
    const char *source = "sample.txt";
    const char *low_copy = "low_level_copy.txt";
    const char *std_copy = "standard_copy.txt";

    /* Create sample input file */
    FILE *file = fopen(source, "w");

    if (file == NULL)
    {
        perror("fopen");
        return 1;
    }

    for (int i = 0; i < 10000; i++)
    {
        fprintf(file, "Operating Systems and Systems Programming - Week 9\n");
    }

    fclose(file);

    clock_t start, end;

    printf("===============================================\n");
    printf("       WEEK 9 - FILE COPY PERFORMANCE\n");
    printf("===============================================\n\n");

    /* Low-level system call copy */
    start = clock();
    low_level_copy(source, low_copy);
    end = clock();

    double low_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Low-level copy completed.\n");
    printf("Functions: open(), read(), write(), lseek(), close()\n");
    printf("Time taken: %.6f seconds\n\n", low_time);

    /* Standard library copy */
    start = clock();
    standard_copy(source, std_copy);
    end = clock();

    double std_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Standard library copy completed.\n");
    printf("Functions: fopen(), fread(), fwrite(), fclose()\n");
    printf("Time taken: %.6f seconds\n\n", std_time);

    printf("Files created:\n");
    printf("1. %s\n", low_copy);
    printf("2. %s\n", std_copy);

    printf("\n===============================================\n");
    printf("             COPY COMPARISON\n");
    printf("===============================================\n");
    printf("Low-level I/O   : %.6f seconds\n", low_time);
    printf("Standard I/O    : %.6f seconds\n", std_time);
    printf("===============================================\n");

    return 0;
}
