#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_OF_THREADS 64
#define LOOPS 1000000
double pi = 0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
void *Monte_Carlo(void *not_used)
{
int i, num_in_circle = 0;
double width, length;
	for(i = 0; i < LOOPS; ++i)
	{
		width = random() / (double)RAND_MAX;
		length = random() / (double)RAND_MAX;
		if(width*width + length*length < 1.0)
			++num_in_circle;	
	}
	pthread_mutex_lock(&mutex1);
	pi += num_in_circle;
	pthread_mutex_unlock(&mutex1);
	pthread_exit(0);
}
int main(int argc, char *argv[])
{
pthread_t tid[NUM_OF_THREADS];
pthread_attr_t attr[NUM_OF_THREADS];
int i;
	for(i = 0; i < NUM_OF_THREADS; ++i)
	{
		pthread_attr_init(&(attr[i]));
		pthread_create(&(tid[i]),&(attr[i]),Monte_Carlo,NULL);
	}
	for(i = 0; i < NUM_OF_THREADS; ++i)
		pthread_join(tid[i],NULL);
	printf("pi = %f\n",pi*4/NUM_OF_THREADS/LOOPS);
	return 0;
}
