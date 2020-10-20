#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#define ENOUGH 10 
int main(int argc, char* argv[])
{
const int SIZE = 4096;
const char *name = "WuSu";
int shm_fd;
void *ptr;
char temp[ENOUGH];
pid_t pid;
int status;
	// Error Checking
	if(argc != 2)
	{
		printf("Wrong Argument\n");
		return 1;
	}
	int Collatz_number = atoi(argv[1]);	
	if(Collatz_number <= 0)
	{
		printf("Input Positive Number\n");
		return 1;
	}
	// Error Checking
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, SIZE);
	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
	pid = fork();	
	if(pid > 0)
	{
		wait(&status);
		printf("%s",(char *)ptr);
		shm_unlink(name);
	}
	else if(pid == 0)
	{
		while(1)
		{
			if(Collatz_number == 1)
			{
				sprintf(temp,"%d\n",Collatz_number);
				strcat(ptr,temp);
				exit(1);
			}
			else
			{
				sprintf(temp,"%d,",Collatz_number);
				strcat(ptr,temp);
			}
			if(Collatz_number % 2 == 0)
				Collatz_number /= 2;
			else
				Collatz_number = Collatz_number * 3 + 1;
		}
		exit(0);
	}
	else
		printf("Something wrongs\n");
	return 0;
}
