#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define INCREMENTS 100000

long counter = 0;
pthread_mutex_t lock;

void *race_condition(void *arg)
{
    for (int i = 0; i < INCREMENTS; i++)
    {
        counter++;
    }

    return NULL;
}

void *mutex_counter(void *arg)
{
    for (int i = 0; i < INCREMENTS; i++)
    {
        pthread_mutex_lock(&lock);

        counter++;

        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main()
{
    pthread_t threads[NUM_THREADS];

    printf("===============================================\n");
    printf("       WEEK 11 - THREADS & MUTEX\n");
    printf("===============================================\n\n");

    printf("Number of threads : %d\n", NUM_THREADS);
    printf("Increments/thread : %d\n", INCREMENTS);
    printf("Expected counter  : %d\n\n",
           NUM_THREADS * INCREMENTS);

    /* Race condition demonstration */
    counter = 0;

    printf("---------- RACE CONDITION ----------\n");

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL,
                       race_condition, NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Counter without mutex : %ld\n", counter);
    printf("Race condition demonstrated.\n\n");

    /* Mutex demonstration */
    counter = 0;

    pthread_mutex_init(&lock, NULL);

    printf("---------- MUTEX PROTECTION ----------\n");

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL,
                       mutex_counter, NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Counter with mutex    : %ld\n", counter);
    printf("Mutex successfully prevented race condition.\n");

    pthread_mutex_destroy(&lock);

    printf("\n===============================================\n");
    printf("Week 11 completed successfully.\n");
    printf("===============================================\n");

    return 0;
}
