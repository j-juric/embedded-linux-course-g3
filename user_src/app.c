#include "common.h"
#include <string.h>

static sem_t semEmpty, semFull, semFinish;

void generate_random_message(char *str, size_t size)
{
    const char alphanum[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int len = (int)(sizeof alphanum - 1);

    for (int i = 0, x; i < size; i++)
    {
        x = rand() % len;
        str[x] = alphanum[x];
    }
    str[size] = '\0';

    return;
}

void generate_message(char *message)
{
    if (DEBUG_MODE == 1)
    {
        strncpy(message, "0000000001", MESSAGE_LENGTH);
    }
    else
    {
        generate_random_message(message, MESSAGE_LENGTH-1);
    }

    return;
}

void *producer(void *param)
{
    char message[MESSAGE_LENGTH];
    int count = 10;
    while (1)
    {
        //Exit condition for debug/test mode
        if (DEBUG_MODE == 1)
        {
            if (sem_trywait(&semFinish) == 0)
            {
                break;
            }
        }

        // Generate message
        generate_message(message);
        printf("Producer: %s\n", message);
        fflush(stdout);

        if (sem_trywait(&semEmpty) == 0)
        {

            //TODO: send message to driver

            sem_post(&semFull);
        }

        // Message delay
        usleep(MESSAGE_DELAY);

        //Countdown for debug/test mode
        if (DEBUG_MODE == 1)
        {
            count--;
            if (count == 0)
            {
                sem_post(&semFinish);
                sem_post(&semFinish);
            }
        }
    }
}

void *consumer(void *param)
{
    char message[MESSAGE_LENGTH];

    while (1)
    {
        //Exit condition for debug/test mode
        if (DEBUG_MODE == 1)
        {
            if (sem_trywait(&semFinish) == 0)
            {
                break;
            }
        }

        if (sem_trywait(&semFull) == 0){

            //TODO: receive message from driver

            printf("Consumer: %s\n", message);
            fflush(stdout);

            sem_post(&semEmpty);
        }

    }
}

int main(int argc, char **argv)
{
    pthread_t tProducer;
    pthread_t tConsumer;

    // Init sempahores
    sem_init(&semEmpty, 0, 1);
    sem_init(&semFull, 0, 0);
    sem_init(&semFinish, 0, 0);

    // Create threads
    pthread_create(&tProducer, NULL, producer, 0);
    pthread_create(&tConsumer, NULL, consumer, 0);

    // Wait for threads to finish
    pthread_join(tProducer, NULL);
    pthread_join(tConsumer, NULL);

    // Destroy sempahores
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    sem_destroy(&semFinish);

    return 0;
}