#include <stdio.h>
#include <stdlib.h>
#define MIN_PID 300
#define MAX_PID 5000
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
			MIN_PID_initial = MIN_PID % 32 - 1;
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
		for(j = MIN_PID_initial; j < remainder; ++j)
		{
			specific_bit = (pid_bitmap[i] & ( 1 << j )) >> j;
			if(specific_bit == 0)
			{
				pid_bitmap[i] = pid_bitmap[i] | ( 1 << j );
				return i*32+(j+1);
			}
		}
	}
	return 1;
}
void release_pid(int pid)
{
	pid_bitmap[pid/32] &= ~( 1 << ( pid % 32 ) );	
}
