#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_OF_THREADS 16
#define LOOPS 100000
double pi[NUM_OF_THREADS];
void *Monte_Carlo(void *pi_estimate)
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
	*(double *)pi_estimate = 4 * (double)num_in_circle / LOOPS;
	pthread_exit(0);
}
int main(int argc, char *argv[])
{
pthread_t tid[NUM_OF_THREADS];
pthread_attr_t attr[NUM_OF_THREADS];
double sum = 0;
int i;
	for(i = 0; i < NUM_OF_THREADS; ++i)
	{
		pthread_attr_init(&(attr[i]));
		pthread_create(&(tid[i]),&(attr[i]),Monte_Carlo,&(pi[i]));
	}
	for(i = 0; i < NUM_OF_THREADS; ++i)
		pthread_join(tid[i],NULL);
	for(i = 0; i < NUM_OF_THREADS; ++i)
		sum += pi[i];
	printf("pi = %f\n",sum/NUM_OF_THREADS);
	return 0;
}
