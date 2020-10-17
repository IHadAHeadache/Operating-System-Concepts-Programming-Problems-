#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <sys/types.h>
#include  <unistd.h>
int  main(void)
{
     int    i;
     pid_t pid = getpid();
     pid_t child_pid;
     fork();
     child_pid = getpid();
     if(pid != child_pid)
	exit(0);
     sleep(5);
     return 0;
}
