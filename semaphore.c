#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

#define N 6

sem_t mutex;
sem_t empty;
sem_t full;

int count =0;
int boxes [6];

void init_semaphores()
{
	sem_init(&mutex, 0, 1);
	sem_init(&empty, 0, N);
	sem_init(&full, 0, 0);
}



int produce_item()
{
   int item;
   printf("-------In produce item------\n");
   item = rand() % 1000001;
   return item;
}


void consume_item(int item)
{
   printf("-------In consume item------ %d \n" , item);
}


void insert_item(int item)
{
   
   boxes[count] = item;
   count ++;
   printf("-------after insert item------%d\n",item);
   
}

 
int remove_item()
{
   int index = count;
   count--;
   printf("-------after remove item------\n");
   return boxes[index];
}



void *producer(void *dummy)
{
	int item;
	int i;
	for(i = 0; i < 40; ++i)
	{
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
	int i;
	for(i = 0; i < 30; ++i)
	{
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
		
	return 0;
}

   

