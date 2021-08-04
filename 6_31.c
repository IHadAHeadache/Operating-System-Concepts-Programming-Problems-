#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#define MAX_RESOURCES 5
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int available_resources = MAX_RESOURCES;
int decrease_count(int count){
	pthread_mutex_lock(&mutex1);
	if(available_resources < count){
		pthread_mutex_unlock(&mutex1);
		return -1;
	}
	else {
		available_resources -= count;
		pthread_mutex_unlock(&mutex1);
		return 0;
	}
}
int increase_count(int count){
	pthread_mutex_lock(&mutex1);
	available_resources += count;
	pthread_mutex_unlock(&mutex1);
	return 0;
}
void *test(void* not_used)
{
	while(1)
	{
		while(decrease_count(3) == -1)
			;
		if(available_resources != 2)
			printf("%d\n",available_resources);
		increase_count(3);
	}
	pthread_exit(NULL);
}
int main(void)
{
pthread_t tid1, tid2;
pthread_attr_t attr; pthread_attr_init(&attr);
	pthread_create(&tid1,&attr,test,NULL);
	pthread_create(&tid2,&attr,test,NULL);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	return 0;
}
