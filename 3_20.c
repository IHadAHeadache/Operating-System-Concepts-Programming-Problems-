#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>

#define BUFFER_SIZE 256
#define READ_END 0
#define WRITE_END 1

int main(int argc, char* argv[])
{
char write_msg[BUFFER_SIZE];
char read_msg[BUFFER_SIZE];
int fd_1[2];
FILE *fp;
pid_t pid;
	if(pipe(fd_1) == -1) {
		fprintf(stderr,"Pipe-1 failed");
		return 1;
	}
	pid = fork();

	if (pid < 0) {
		fprintf(stderr,"Fork failed");
		return 1;
	}
	else if (pid > 0) {
		if((fp=fopen(argv[1],"r"))==NULL){
          		printf("open file error!!\n");
          		system("PAUSE");
	          	exit(0);
      		}    
		close(fd_1[READ_END]);
		while(!feof(fp))
		{
			if(fgets(write_msg,BUFFER_SIZE,fp) != NULL)
				write(fd_1[WRITE_END], write_msg, strlen(write_msg));
		}
		close(fd_1[WRITE_END]);
		fclose(fp);
	}
	else {
		close(fd_1[WRITE_END]);
		FILE *fp = fopen(argv[2],"w");
		FILE *fp_1 = fdopen(fd_1[READ_END],"r");
		char ch;
		while((ch=fgetc(fp_1))!=EOF)
			fputc(ch,fp);
		close(fd_1[READ_END]);
		fclose(fp);
	}
	return 0;
}
