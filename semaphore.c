#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

#define N 6
#define THREAD_PRODUCE 3
#define THREAD_CONSUME 4
static int producerCounter = 120;
static int consumerCounter = 120;
sem_t mutex;
sem_t mutex_con;
sem_t mutex_pro;
sem_t empty;
sem_t full;

int index = 0;
int box [N];

int reminder = 120;

void init_semaphores()
{
    sem_init(&mutex_con, 0, 1);
    sem_init(&mutex_pro, 0, 1);
	sem_init(&mutex, 0, 1);
	sem_init(&empty, 0, N);
	sem_init(&full, 0, 0);
}



int produce_item()
{
    int item = rand() % 1000001;
    printf("===========Produce item: %d==========\n", item);
    return item;
}

void consume_item(int item)
{
    printf("===========Consume item: %d==========\n", item);
}

void insert_item(int item)
{
    box[index] = item;
    ++index;
    printf("===========insert item %d to the box==========\n", item);
}

int remove_item()
{
    --index;
    printf("===========remove item %d to the box==========\n", box[index]);
    return box[index];
}


void *producer(void *dummy)
{
    int item;

    while (producerCounter > 0)
    {
        sem_wait(&mutex_con);
        if (producerCounter < 0)
        {
            sem_post(&mutex_con);
            break;
        }
        --producerCounter;
        sem_post(&mutex_con);

        item = produce_item();
        sem_wait(&empty);
        sem_wait(&mutex);
        insert_item(item);
        sem_post(&mutex);
        sem_post(&full);
    }

    pthread_exit(NULL);
}


void *consumer(void *dummy)
{
    int item;
    while (consumerCounter > 0)
    {
        sem_wait(&mutex_con);
        if (consumerCounter < 0)
        {
            sem_post(&mutex_con);
            break;
        }
        --consumerCounter;
        sem_post(&mutex_con);
        sem_wait(&full);
        sem_wait(&mutex);
        item = remove_item();
        sem_post(&mutex);
        sem_post(&empty);
        consume_item(item);
    }
    pthread_exit(NULL);
}




int main()
{
    int i;
    pthread_t producers[3];
    pthread_t consumers[4];

    init_semaphores();

    for(i = 0; i < 3; ++i)
        pthread_create(&producers[i], NULL, producer, NULL);

    for(i = 0; i < 4; ++i)
        pthread_create(&consumers[i], NULL, consumer, NULL);

    for(i = 0; i < 3; ++i)
        pthread_join(producers[i], NULL);

    for(i = 0; i < 4; ++i)
        pthread_join(consumers[i], NULL);

    printf("Done!!!!\n");
    return 0;
}
   

