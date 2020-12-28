#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define N 4
struct prime_list {
	int prime;
	struct prime_list *next;
};
struct prime_list *head, *iterator;
void *prime_test(void *number)
{
int num = *(int *)number;
struct prime_list *temp = head;
int i, j;
	for(j = 1; j < num; ++j)
		temp = temp->next;
	for(i = 2; i < num; ++i)
		if(num % i == 0)
		{
			temp->prime = 0;
			pthread_exit(0);
		}
	temp->prime = 1;
	pthread_exit(0);
}
int main(int argc, char* argv[])
{
if(argc != 2)
	fprintf(stderr,"Wrong Argument.\n");
int to_num = atoi(argv[1]);
pthread_t tid[N];
pthread_attr_t attr; pthread_attr_init(&attr);
int arg[N];
int i, j;
	head = malloc(sizeof(struct prime_list));
	iterator = head;
	for(i = 1; i < to_num; ++i)
	{
		iterator->next = malloc(sizeof(struct prime_list));
		iterator = iterator->next;
		iterator->next = NULL;
	}
	for(i = 0; i < to_num; i=i+N)
	{
		if(i+N > to_num)
		{
			for(j = 0; j < to_num % N; ++j)
			{
				arg[j] = i+(j+1);
				pthread_create(&(tid[j]),&attr,prime_test,arg+j);
			}
			for(j = 0; j < to_num % N; ++j)
				pthread_join(tid[j],NULL);
		}
		else
		{
			for(j = 0; j < N; ++j)
			{
				arg[j] = i+(j+1);
				pthread_create(&(tid[j]),&attr,prime_test,&(arg[j]));
			}
			for(j = 0; j < N; ++j)
				pthread_join(tid[j],NULL);
		}
	}
	iterator = head->next;
	int count = 2;
	while(iterator != NULL)
	{
		if(iterator->prime == 1)
			printf("%d ",count);
		++count;
		iterator = iterator->next;
	}
	printf("\n");
	while(head != NULL)
	{
		iterator = head->next;
		free(head);
		head = iterator;
	}
	return 0;
}
