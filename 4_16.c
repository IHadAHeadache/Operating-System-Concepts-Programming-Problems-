#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
struct num {
	double value;
	struct num *next;
};
double average;
double minimum;
double maximum;
void *calculate_average(void* num)
{
double sum = 0;
double num_element = 0;
	while(num != NULL)
	{
		sum = sum + ((struct num*)num)->value;
		num = ((struct num*)num)->next;
		num_element = num_element + 1;
	}
	average = sum / num_element;
	pthread_exit(0);
}
void *calculate_minimum(void* num)
{
double minist = ((struct num*)num)->value;
	while(num != NULL)
	{
		if(minist > ((struct num*)num)->value)
			minist = ((struct num*)num)->value;
		num = ((struct num*)num)->next;
	}
	minimum = minist;
	pthread_exit(0);
}
void *calculate_maximum(void* num)
{
double maxist = ((struct num*)num)->value;
	while(num != NULL)
	{
		if(maxist < ((struct num*)num)->value)
			maxist = ((struct num*)num)->value;
		num = ((struct num*)num)->next;
	}
	maximum = maxist;
	pthread_exit(0);
}
int main(int argc, char* argv[])
{
pthread_t tid_average;	
pthread_t tid_minimum;	
pthread_t tid_maximum;
pthread_attr_t attr_average;
pthread_attr_t attr_minimum;
pthread_attr_t attr_maximum;
struct num* head = calloc(1,sizeof(struct num)); head->next = NULL;
struct num* tail = head;
struct num* pre_head = head;
int i = 1;
	while(--argc > 0)
	{
		tail->next = malloc(sizeof(struct num)); tail = tail->next;
		tail->value = atof(argv[i++]);
		tail->next = NULL;
	}
	pthread_attr_init(&attr_average);
	pthread_attr_init(&attr_minimum);
	pthread_attr_init(&attr_maximum);
	pthread_create(&tid_average,&attr_average,calculate_average,head->next);
	pthread_create(&tid_minimum,&attr_minimum,calculate_minimum,head->next);
	pthread_create(&tid_maximum,&attr_maximum,calculate_maximum,head->next);
	pthread_join(tid_average,NULL);
	pthread_join(tid_minimum,NULL);
	pthread_join(tid_maximum,NULL);
	printf("The average value is %.2f\n",average);
	printf("The minimum value is %.2f\n",minimum);
	printf("The maximum value is %.2f\n",maximum);
	while(head != NULL)
	{
		head = head->next;
		free(pre_head);
		pre_head = head;
	}
	return 0;
}
