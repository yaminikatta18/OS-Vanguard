#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int main()
{
    const char *filename = "inode_sample.txt";
    struct stat file_stat;

    /* Create a sample file */
    FILE *file = fopen(filename, "w");

    if (file == NULL)
    {
        perror("fopen");
        return 1;
    }

    fprintf(file, "This is an inode demonstration file for Week 10.\n");
    fclose(file);

    /* Get inode information */
    if (stat(filename, &file_stat) == -1)
    {
        perror("stat");
        return 1;
    }

    printf("===============================================\n");
    printf("          WEEK 10 - INODE STRUCTURE\n");
    printf("===============================================\n\n");

    printf("File name       : %s\n", filename);
    printf("Inode number    : %lu\n",
           (unsigned long)file_stat.st_ino);
    printf("File size       : %ld bytes\n",
           (long)file_stat.st_size);
    printf("Hard links      : %lu\n",
           (unsigned long)file_stat.st_nlink);
    printf("User ID         : %u\n",
           file_stat.st_uid);
    printf("Group ID        : %u\n",
           file_stat.st_gid);
    printf("Permissions     : %o\n",
           file_stat.st_mode & 0777);
    printf("Block size      : %ld bytes\n",
           (long)file_stat.st_blksize);
    printf("Blocks allocated: %ld\n",
           (long)file_stat.st_blocks);

    printf("\nFile type:\n");

    if (S_ISREG(file_stat.st_mode))
        printf("Regular file\n");
    else if (S_ISDIR(file_stat.st_mode))
        printf("Directory\n");
    else if (S_ISLNK(file_stat.st_mode))
        printf("Symbolic link\n");
    else
        printf("Other file type\n");

    printf("\n===============================================\n");
    printf("An inode stores metadata about a file.\n");
    printf("The inode does not store the file name itself.\n");
    printf("===============================================\n");

    return 0;
}
