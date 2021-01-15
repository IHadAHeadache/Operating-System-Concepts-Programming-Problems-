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
time_t t;  
struct tm tm;
int valread;
	while( (valread = read( *(int *)new_socket_ptr , buffer, 1024)) > 0)
	{
		t = time(NULL);
		tm = *localtime(&t);
		sprintf(buffer,"now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
   		send(*(int *)new_socket_ptr , buffer , strlen(buffer) , 0 );
		memset(buffer,'\0',strlen(buffer));
	}
	printf("Connection lost:%d\n",*(int *)new_socket_ptr);
	pthread_exit(0);
}
int main(int argc, char const *argv[]) 
{ 
pthread_t tid[NUMBER_OF_THREADS];
pthread_attr_t attr; pthread_attr_init(&attr);
int server_fd, new_socket, valread; 
int new_socket_ptr[NUMBER_OF_THREADS];
struct sockaddr_in address; 
int opt = 1; 
int addrlen = sizeof(address); 
char buffer[1024] = {0}; 
char *hello = "Hello from server"; 
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
    	printf("Incoming socket:%d\n",new_socket);
	new_socket_ptr[i] = new_socket;
	pthread_create(&(tid[i]),&attr,Date_serve,new_socket_ptr+i);
	i = (i + 1) % 3;
    }
    return 0; 
}

