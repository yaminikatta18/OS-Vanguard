#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int global_var = 100;
static int static_var = 200;

void code_function()
{
    printf("Code/Function address  : %p\n", (void *)code_function);
}

int main()
{
    int stack_var = 300;
    int *heap_var = (int *)malloc(sizeof(int));

    if (heap_var == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    *heap_var = 400;

    printf("===============================================\n");
    printf("       WEEK 7 - PROCESS ADDRESS SPACE\n");
    printf("===============================================\n");

    printf("Process ID (PID)       : %d\n", getpid());

    code_function();

    printf("Global variable address : %p\n", (void *)&global_var);
    printf("Static variable address : %p\n", (void *)&static_var);
    printf("Heap variable address   : %p\n", (void *)heap_var);
    printf("Stack variable address  : %p\n", (void *)&stack_var);

    printf("\nMemory Segments:\n");
    printf("Code/Text : Program instructions\n");
    printf("Data      : Global and static variables\n");
    printf("Heap      : Dynamically allocated memory\n");
    printf("Stack     : Local variables and function calls\n");

    printf("\nCheck virtual memory mappings using:\n");
    printf("cat /proc/%d/maps\n", getpid());

    printf("\nProcess is running. Press Enter to exit...\n");
    getchar();

    free(heap_var);

    printf("===============================================\n");

    return 0;
}
