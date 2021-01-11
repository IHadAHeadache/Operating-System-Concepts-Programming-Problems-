#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX 406
unsigned long long int Fib_array[MAX] = {0,1};
void *Fib_calculator(void *Fibonacci_number)
{
int n = atoi(Fibonacci_number);
int n_copy = n + 1;
	while(--n > 0)
		Fib_array[n_copy - n] = Fib_array[n_copy - n - 1] + Fib_array[n_copy - n - 2];
	pthread_exit(0);
}
int main(int argc, char *argv[])
{
pthread_t tid;
pthread_attr_t attr; pthread_attr_init(&attr);
int i;
	if(argc != 2)
	{
		fprintf(stderr,"Wrong Argument!\n");
		exit(0);
	}
	if(atoi(argv[1]) > MAX)
	{
		fprintf(stderr,"Sorry, argument > %d is not supported.\n",MAX);
		exit(0);
	}
	pthread_create(&tid,&attr,Fib_calculator,argv[1]);
	pthread_join(tid,NULL);
	for(i = 0; i <= atoi(argv[1]); ++i)
		printf("%llu ",Fib_array[i]);
	printf("\n");
	return 0;
}
