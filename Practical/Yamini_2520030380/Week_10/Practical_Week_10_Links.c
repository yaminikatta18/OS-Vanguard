#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int main()
{
    const char *original = "original.txt";
    const char *hard_link = "hard_link.txt";
    const char *symbolic_link = "symbolic_link.txt";

    struct stat original_stat, hard_stat, symbolic_stat;

    /* Create original file */
    FILE *file = fopen(original, "w");

    if (file == NULL)
    {
        perror("fopen");
        return 1;
    }

    fprintf(file, "This is the original file for Week 10 links.\n");
    fclose(file);

    /* Create hard link */
    if (link(original, hard_link) == -1)
    {
        perror("link");
        return 1;
    }

    /* Create symbolic link */
    if (symlink(original, symbolic_link) == -1)
    {
        perror("symlink");
        return 1;
    }

    /* Get inode information */
    if (stat(original, &original_stat) == -1 ||
        stat(hard_link, &hard_stat) == -1 ||
        lstat(symbolic_link, &symbolic_stat) == -1)
    {
        perror("stat");
        return 1;
    }

    printf("===============================================\n");
    printf("       WEEK 10 - HARD & SYMBOLIC LINKS\n");
    printf("===============================================\n\n");

    printf("Original file inode : %lu\n",
           (unsigned long)original_stat.st_ino);

    printf("Hard link inode     : %lu\n",
           (unsigned long)hard_stat.st_ino);

    printf("Symbolic link inode : %lu\n",
           (unsigned long)symbolic_stat.st_ino);

    printf("\nHard link count     : %lu\n",
           (unsigned long)original_stat.st_nlink);

    printf("\n-----------------------------------------------\n");
    printf("Hard Link:\n");
    printf("- Points directly to the same inode.\n");
    printf("- Original and hard link have the same inode.\n");

    printf("\nSymbolic Link:\n");
    printf("- Stores a path to the target file.\n");
    printf("- Symbolic link has its own inode.\n");

    printf("\n-----------------------------------------------\n");

    if (original_stat.st_ino == hard_stat.st_ino)
        printf("Result: Hard link and original share the same inode.\n");

    if (original_stat.st_ino != symbolic_stat.st_ino)
        printf("Result: Symbolic link has a different inode.\n");

    printf("===============================================\n");

    return 0;
}
