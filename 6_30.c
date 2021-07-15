#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#define MIN_PID 300
#define MAX_PID 5000
#define MAXIMUM_THREADS 512
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
unsigned int *pid_bitmap;
int allocate_map(void)
{
	pid_bitmap = calloc(MAX_PID/32+1,sizeof(unsigned int));
	if(pid_bitmap == NULL)
		return 1;
	else
		return 0;
}
int allocate_pid(void)
{
int i, j;
int remainder;
int MIN_PID_initial;
int specific_bit;
	for(i=MIN_PID/32;i<=MAX_PID/32;++i)
	{
		if(i==MIN_PID/32)
		{
			MIN_PID_initial = MIN_PID % 32;
			remainder = 32 - (MIN_PID % 32);
		}
		else if(i==MAX_PID/32)
		{
			MIN_PID_initial = 0;
			remainder = MAX_PID % 32;
		}
		else
		{
			MIN_PID_initial = 0;
			remainder = 32;
		}
		for(j = MIN_PID_initial; j < MIN_PID_initial + remainder; ++j)
		{
			pthread_mutex_lock(&mutex);
			specific_bit = (pid_bitmap[i] & ( 1 << j )) >> j;
			if(specific_bit == 0)
			{
				pid_bitmap[i] = pid_bitmap[i] | ( 1 << j );
				pthread_mutex_unlock(&mutex);
				return i*32+j;
			}
			pthread_mutex_unlock(&mutex);
		}
	}
	return 1;
}
void release_pid(int pid)
{
	pid_bitmap[pid/32] &= ~( 1 << ( pid % 32 ) );	
}
void* pid_request(void *not_use)
{
int a, pid;
	pid = allocate_pid();
	srand(time(NULL)+pid%10+(pid%100-pid%10)/10);
	a = (rand()%10) + 1;
	sleep(a);
	release_pid(pid);
	printf("pid:%d sleep for %d seconds\n",pid,a);
	pthread_exit(0);
}
int main(int argc, char *argv[])
{
int num_threads = atoi(argv[1]);
int i;
pthread_t tid[MAXIMUM_THREADS];
pthread_attr_t attr[MAXIMUM_THREADS];
	if(argc != 2)
	{
		fprintf(stderr,"usage: a.out <integer value>\n");
		return -1;
	}
	if(num_threads < 0 || num_threads > MAXIMUM_THREADS)
	{
		fprintf(stderr,"number of threads must >= 0 && <= 512\n");
		return -1;
	}
	if(allocate_map())
	{
		fprintf(stderr,"Fail to initialize pid manager\n");
		return -1;
	}
	for(i = 0; i < num_threads; ++i)
	{
		pthread_attr_init(&(attr[i]));
		pthread_create(&(tid[i]),&(attr[i]),pid_request,NULL);
	}
	for(i = 0; i < num_threads; ++i)
		pthread_join(tid[i],NULL);
	return 0;
}
