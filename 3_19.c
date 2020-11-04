#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define BUFFER_SIZE 256
#define READ_END 0
#define WRITE_END 1

int main(void)
{
char write_msg[BUFFER_SIZE];
char read_msg[BUFFER_SIZE];
int fd_1[2];
int fd_2[2];
pid_t pid;
	if(pipe(fd_1) == -1) {
		fprintf(stderr,"Pipe1 failed");
		return 1;
	}
	else if(pipe(fd_2) == -1) {
		fprintf(stderr,"Pipe2 failed");
		return 1;
	}

	pid = fork();

	if (pid < 0) {
		fprintf(stderr,"Fork failed");
		return 1;
	}

	if (pid > 0) {
		close(fd_1[READ_END]);
		close(fd_2[WRITE_END]);
		fgets(write_msg,BUFFER_SIZE,stdin);
		write(fd_1[WRITE_END], write_msg, strlen(write_msg)+1);
		close(fd_1[WRITE_END]);
		FILE *fp_2 = fdopen(fd_2[READ_END],"r");
		fgets(read_msg,BUFFER_SIZE,fp_2);
		printf("%s",read_msg);
		close(fd_2[READ_END]);
	}
	else {
		close(fd_1[WRITE_END]);
		close(fd_2[READ_END]);
		FILE *fp_1 = fdopen(fd_1[READ_END],"r");
		fgets(read_msg,BUFFER_SIZE,fp_1);
		int i;
		for(i = 0; i < strlen(read_msg); ++i)
			if(islower(read_msg[i]))
				write_msg[i] = toupper(read_msg[i]);
			else if(isupper(read_msg[i]))
				write_msg[i] = tolower(read_msg[i]);
			else 
				write_msg[i] = read_msg[i];
		write_msg[i] = '\0';
		close(fd_1[READ_END]);
		write(fd_2[WRITE_END], write_msg, strlen(write_msg)+1);
		close(fd_2[WRITE_END]);
	}
	return 0;
}
