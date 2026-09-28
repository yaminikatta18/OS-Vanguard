#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define ITEMS 10

int buffer[BUFFER_SIZE];

int in = 0;
int out = 0;

sem_t empty;
sem_t full;

pthread_mutex_t mutex;

void *producer(void *arg)
{
    for (int i = 1; i <= ITEMS; i++)
    {
        sem_wait(&empty);

        pthread_mutex_lock(&mutex);

        buffer[in] = i;
        printf("Producer: Produced item %d at buffer[%d]\n", i, in);
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);

        sem_post(&full);

        usleep(100000);
    }

    return NULL;
}

void *consumer(void *arg)
{
    for (int i = 1; i <= ITEMS; i++)
    {
        sem_wait(&full);

        pthread_mutex_lock(&mutex);

        int item = buffer[out];
        printf("Consumer: Consumed item %d from buffer[%d]\n",
               item, out);

        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);

        sem_post(&empty);

        usleep(150000);
    }

    return NULL;
}

int main()
{
    pthread_t producer_thread;
    pthread_t consumer_thread;

    printf("===============================================\n");
    printf("     WEEK 12 - PRODUCER CONSUMER & DEADLOCK\n");
    printf("===============================================\n\n");

    printf("Buffer size       : %d\n", BUFFER_SIZE);
    printf("Items produced    : %d\n", ITEMS);
    printf("Synchronization   : Counting Semaphores\n\n");

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    pthread_mutex_init(&mutex, NULL);

    printf("---------- PRODUCER-CONSUMER ----------\n");

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    printf("\nProducer and Consumer completed successfully.\n");

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    printf("\n---------- DEADLOCK SCENARIO ----------\n");

    printf("Four necessary conditions for deadlock:\n");
    printf("1. Mutual Exclusion\n");
    printf("2. Hold and Wait\n");
    printf("3. No Preemption\n");
    printf("4. Circular Wait\n");

    printf("\nDeadlock prevention strategy:\n");
    printf("- Use a fixed resource ordering.\n");
    printf("- Threads must acquire resources in the same order.\n");
    printf("- This prevents circular wait.\n");

    printf("\n===============================================\n");
    printf("       WEEK 12 COMPLETED SUCCESSFULLY\n");
    printf("===============================================\n");

    return 0;
}
