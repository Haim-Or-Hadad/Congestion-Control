#include <sys/socket.h>
#include <netinet/in.h> 
#include <netinet/tcp.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h>
#include <string.h> 
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>


#define SERVER_PORT 5093 
#define SIZE 65536

static time_t total_time = 0;
static double avrg_time = 0;



int main(int argc, char const *argv[]) 
{ 
char buffer[SIZE] = {0};
int listeningSocket = -1;
if((listeningSocket=socket(AF_INET , SOCK_STREAM , 0)) == -1){
printf("could not create a listening socket: %d", errno);
}

int enableReuse = 1;
if(setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, &enableReuse, sizeof(enableReuse))<0){
    printf("failed : %d",errno);
}
struct sockaddr_in serverAddress ,Cadrress;
socklen_t lenClientAddress = sizeof(Cadrress);
memset(&serverAddress, 0,sizeof(serverAddress));

serverAddress.sin_family = AF_INET;
serverAddress.sin_addr.s_addr = INADDR_ANY;
serverAddress.sin_port = htons(SERVER_PORT);

if(bind(listeningSocket, (struct sockaddr *)&serverAddress , sizeof(serverAddress)) == -1){
    printf("Bind failed with error code");
}

printf("Bind function success\n");

if(listen(listeningSocket , 5) == -1)
{
    printf("listening not success");
    close(listeningSocket);
    return -1;
}

printf("waiting for tcp connections......\n");


int i =0;
int bytesGot = 0;
char bufcc[256];
socklen_t bufcclen = sizeof(bufcc);
if (getsockopt(listeningSocket, IPPROTO_TCP, TCP_CONGESTION, buffer, &bufcclen) != 0) 
		{
			perror("fail in getsockopt"); 
			return -1; 
		}
int Csocket = -1;        
while(i<5)
{
    memset(&Cadrress , 0 , sizeof(Cadrress));
    lenClientAddress = sizeof(Cadrress);
    Csocket = accept(listeningSocket, (struct sockaddr *)&Cadrress, &lenClientAddress);
    if(Csocket == -1){
        printf("accept failed: %d",errno);
        close(listeningSocket);
        return -1;
    }
    printf("A new client connection accepted\n");
    clock_t begin_time=clock();
    time_t stime =time(0);
    int tot=0;
        
    while ((bytesGot = recv(Csocket,buffer,sizeof(buffer),0))>0) {
            tot+=bytesGot;
        }
    clock_t end_time=clock();
    time_t endtime =time(0);
    double time_spent = (double)(end_time - begin_time) / CLOCKS_PER_SEC;
    total_time +=endtime-stime;
    avrg_time+=time_spent;    
    printf("Received: %d bytes in %f seconds \t", tot, time_spent);
	printf("(%ld seconds) \n", endtime-stime);
			
	if (bytesGot<0)
	    perror("failed to get the file!");
	
	i++;
    close(Csocket);

}   
    printf("total avarage time is  %f\n",avrg_time/5);
    printf("total general time is %ld seconds\n",total_time);
    strcpy(buffer, "reno");
    bufcclen=sizeof(buffer);
    if (setsockopt(listeningSocket, IPPROTO_TCP, TCP_CONGESTION, buffer, bufcclen) != 0) {
			perror("setsockopt"); 
			return -1;
    }
    if (getsockopt(listeningSocket, IPPROTO_TCP, TCP_CONGESTION, buffer, &bufcclen) != 0) {
			perror("getsockopt"); 
			return -1; 
		}
     printf("New CC: %s\n", buffer); 
     bytesGot=0;
     total_time=0;
     avrg_time=0;
     i=0;
     while(i<5){
        memset(&Cadrress , 0 , sizeof(Cadrress));
        lenClientAddress = sizeof(Cadrress);
        int clientSocket = accept(listeningSocket, (struct sockaddr *)&Cadrress, &lenClientAddress);
        if(clientSocket == -1){
            printf("accept failed: %d",errno);
            close(listeningSocket);
            return -1;
        }
    clock_t begin_time=clock();
    time_t stime =time(0);
    int tot=0;
      while( (bytesGot = recv(clientSocket, buffer, sizeof(buffer),0))> 0 ) {
	            tot += bytesGot;
	        }
        clock_t end_time=clock();
        time_t endtime =time(0);
        double time_spent = (double)(end_time - begin_time) / CLOCKS_PER_SEC;
        total_time +=endtime-stime;
        avrg_time+=time_spent;    
        printf("Got: %d bytes in %f seconds \t", tot, time_spent);
			printf("(%ld seconds) \n", endtime-stime);
			
	        if (bytesGot<0)
	        	perror("failed to get the file!");
	
	        i++;
        close(Csocket);

}   
    printf("total avarage time is  %f\n",avrg_time/5);
    printf("total general time is %ld seconds\n",total_time);
    return 0;
    
    }


