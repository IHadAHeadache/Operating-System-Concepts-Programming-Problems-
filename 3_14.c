#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
int main(int argc, char* argv[])
{
	int status;
	pid_t pid;
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
	pid = fork();
	if(pid < 0)
	{
		printf("Something wrongs\n");
		return 1;
	}
	else if(pid == 0)
	{
		while(1)
		{
			if(Collatz_number == 1)
			{
				printf("%d\n",Collatz_number);
				exit(0);
			}
			else
				printf("%d,",Collatz_number);
			if(Collatz_number % 2 == 0)
				Collatz_number /= 2;
			else
				Collatz_number = Collatz_number * 3 + 1;
		}
	}
	else
	{
		wait(&status);
		if (!WIFEXITED(status))
			printf("Status: %d \n",WEXITSTATUS(status));
		return 0;
	}
}
