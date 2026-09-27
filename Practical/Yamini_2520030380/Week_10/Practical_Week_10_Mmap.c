#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

int main()
{
    const char *filename = "mmap_file.txt";
    const char *message = "Hello from memory-mapped file I/O - Week 10!\n";
    const char *modified_message = "Modified using mmap() - Week 10!\n";

    int fd;
    struct stat file_stat;
    char *mapped;

    printf("===============================================\n");
    printf("        WEEK 10 - mmap() FILE I/O\n");
    printf("===============================================\n\n");

    fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);

    if (fd < 0)
    {
        perror("open");
        return 1;
    }

    if (write(fd, message, strlen(message)) == -1)
    {
        perror("write");
        close(fd);
        return 1;
    }

    if (fstat(fd, &file_stat) == -1)
    {
        perror("fstat");
        close(fd);
        return 1;
    }

    size_t file_size = file_stat.st_size;

    mapped = mmap(NULL, file_size,
                  PROT_READ | PROT_WRITE,
                  MAP_SHARED, fd, 0);

    if (mapped == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        return 1;
    }

    printf("File name       : %s\n", filename);
    printf("File size       : %zu bytes\n", file_size);
    printf("Mapped address  : %p\n", (void *)mapped);

    printf("\nOriginal content:\n");
    printf("%s", mapped);

    /* Clear old content */
    memset(mapped, 0, file_size);

    /* Write new content through mapped memory */
    memcpy(mapped, modified_message, strlen(modified_message));

    /* Save changes to file */
    if (msync(mapped, file_size, MS_SYNC) == -1)
    {
        perror("msync");
    }

    printf("\nModified content through mmap():\n");
    printf("%s", mapped);

    /* Remove mapping */
    if (munmap(mapped, file_size) == -1)
    {
        perror("munmap");
    }

    close(fd);

    printf("\n===============================================\n");
    printf("mmap() successfully demonstrated file mapping.\n");
    printf("Changes were written back to the file.\n");
    printf("===============================================\n");

    return 0;
}
