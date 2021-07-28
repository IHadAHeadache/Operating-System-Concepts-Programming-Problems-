#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#define MAX_RESOURCES 5
sem_t semaphore;
int available_resources = MAX_RESOURCES;
int decrease_count(int count){
	sem_wait(&semaphore);
	if(available_resources < count){
		sem_post(&semaphore);
		return -1;
	}
	else {
		available_resources -= count;
		sem_post(&semaphore);
		return 0;
	}
}
int increase_count(int count){
	sem_wait(&semaphore);
	available_resources += count;
	sem_post(&semaphore);
	return 0;
}
void *test(void* not_used)
{
	while(1)
	{
		decrease_count(3);
		if(available_resources < 0)
			printf("%d\n",available_resources);
		increase_count(3);
	}
	pthread_exit(NULL);
}
int main(void)
{
pthread_t tid1, tid2;
pthread_attr_t attr; pthread_attr_init(&attr);
sem_open("semaphore", O_CREAT|O_EXCL,S_IRWXU, 1);
	pthread_create(&tid1,&attr,test,NULL);
	pthread_create(&tid2,&attr,test,NULL);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	return 0;
}
