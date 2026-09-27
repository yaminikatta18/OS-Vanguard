#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("===============================================\n");
    printf("       WEEK 8 - DYNAMIC MEMORY ALLOCATION\n");
    printf("===============================================\n");

    // malloc()
    int *a = (int *)malloc(3 * sizeof(int));

    if (a == NULL)
    {
        printf("malloc failed.\n");
        return 1;
    }

    a[0] = 10;
    a[1] = 20;
    a[2] = 30;

    printf("\n1. malloc():\n");
    printf("Allocated 3 integers: %d %d %d\n", a[0], a[1], a[2]);

    // calloc()
    int *b = (int *)calloc(3, sizeof(int));

    if (b == NULL)
    {
        printf("calloc failed.\n");
        free(a);
        return 1;
    }

    printf("\n2. calloc():\n");
    printf("Allocated 3 integers initialized to: %d %d %d\n",
           b[0], b[1], b[2]);

    // realloc()
    a = (int *)realloc(a, 5 * sizeof(int));

    if (a == NULL)
    {
        printf("realloc failed.\n");
        free(b);
        return 1;
    }

    a[3] = 40;
    a[4] = 50;

    printf("\n3. realloc():\n");
    printf("Expanded memory to 5 integers: ");

    for (int i = 0; i < 5; i++)
    {
        printf("%d ", a[i]);
    }

    printf("\n");

    // free()
    free(a);
    free(b);

    printf("\n4. free():\n");
    printf("Allocated memory released successfully.\n");

    printf("\nMemory allocation demonstration completed.\n");
    printf("===============================================\n");

    return 0;
}
