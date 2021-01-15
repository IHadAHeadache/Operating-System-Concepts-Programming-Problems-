#include <pthread.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <time.h>
#define PORT 6013 
#define NUMBER_OF_THREADS 3
void *Date_serve(void *new_socket_ptr)
{
char buffer[1024] = {0}; 
int valread;
	while((valread = read(*(int *)new_socket_ptr,buffer,1024))>0)
	{
		printf("%s",buffer);
   		if((valread = send(*(int *)new_socket_ptr , buffer , strlen(buffer) , 0 )) == -1)
			break;
		memset(buffer,'\0',strlen(buffer));
	}
	printf("Connection lost:%d\n",*(int *)new_socket_ptr);
	*(int *)new_socket_ptr = 0;
	pthread_exit(0);
}
int main(int argc, char const *argv[]) 
{ 
pthread_t tid[NUMBER_OF_THREADS];
pthread_attr_t attr; pthread_attr_init(&attr);
int server_fd, new_socket, valread; 
int new_socket_ptr[NUMBER_OF_THREADS] = {0};
struct sockaddr_in address; 
int opt = 1; 
int addrlen = sizeof(address); 
char buffer[1024] = {0}; 
int i = 0;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 0) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    }
    while(( new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))>=0)
    {
    int while_continuer = 0;
    	for(i = 0; i < NUMBER_OF_THREADS; i++)
	{
		if(new_socket_ptr[i] == 0)
		{
			new_socket_ptr[i] = new_socket;
			break;
		}
		if(i == NUMBER_OF_THREADS - 1)
		{
			while_continuer = 1;
   			send(new_socket , "Server busy!\n" , 14 , 0 );
			close(new_socket);
		}
	}
	if(while_continuer == 1)
		continue;
    	printf("Incoming socket:%d\n",new_socket);
	pthread_create(&(tid[i]),&attr,Date_serve,new_socket_ptr+i);
    }
    return 0; 
}

