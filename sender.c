#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <netinet/in.h> 
#include <netinet/tcp.h>
#include <time.h>
#include <errno.h> 
#include <string.h> 

#define PORT 5093 
#define SERVER_IP_ADDRESS "127.0.0.1"
#define SIZE 65536
int main(int argc, char const *argv[]) 
{ 
	int sock = 0; 
    struct sockaddr_in serv_addr;
    
	for(int i = 0; i < 5; i ++){
    	char buffer[SIZE] = {0};
        sock = socket(AF_INET, SOCK_STREAM, 0);
    	if (sock < 0) 
    	{ 
        	printf("\n Error in socket creation \n"); 
        	return -1; 
    	} 
    	
   		bzero(&serv_addr, sizeof(serv_addr));
    	serv_addr.sin_family = AF_INET; 
    	serv_addr.sin_port = htons(PORT); 
        if(inet_pton(AF_INET, (const char*)SERVER_IP_ADDRESS, &serv_addr.sin_addr)<=0)  
    	{ 
        	printf("\ninet_pton() failed.\n"); 
        	return -1; 
    	} 
    		
		printf("connecting to server...\n");
    	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    	{ 
        	printf("connecting to server failed with error code : %d \n", errno); 
        	return -1; 
    	} 
        printf("connected...\n");
        
    	FILE *txt_file; 
		txt_file = fopen("1gb.txt", "r");
        if(txt_file == NULL){
        	perror("Error in reading file.");
        	return -1;
        }
        printf("Sending file %d...\n",i+1);
        int data_chunk = 0;
        int Bytes_Sent = 0;
        int counter = 0;
        int Seg_size = 0;
    	while((data_chunk = fread(buffer, 1, sizeof(buffer), txt_file))>0 ){
	   		 Bytes_Sent = send(sock, buffer, data_chunk, 0);
			 if (Bytes_Sent == -1)
		     {
				printf("Sending file Error: %d", errno);
		     }
		     else if (Bytes_Sent == 0)
		     {
				printf("TCP connection closed before send() function.\n");
		     }
		     else if ( Bytes_Sent < data_chunk) 
		     {
				printf("Error: sent %d bytes from the %d required .\n", Bytes_Sent, data_chunk);
		     }
		     counter+=Bytes_Sent;
		     Seg_size+=1;
   		}
   		printf("Send %d bytes of file %d by %d segments.\n", counter, i+1, Seg_size);
		fclose(txt_file);
		close(sock);
	}

		for(int i = 0; i < 5; i ++){
    	char buffer[SIZE] = {0};
        sock = socket(AF_INET, SOCK_STREAM, 0);
    	if (sock < 0) 
    	{ 
        	printf("\n Error in socket creation \n"); 
        	return -1; 
    	} 
    	
   		bzero(&serv_addr, sizeof(serv_addr));
    	serv_addr.sin_family = AF_INET; 
    	serv_addr.sin_port = htons(PORT); 
        if(inet_pton(AF_INET, (const char*)SERVER_IP_ADDRESS, &serv_addr.sin_addr)<=0)  
    	{ 
        	printf("\ninet_pton() failed.\n"); 
        	return -1; 
    	} 
    		
		strcpy(buffer, "reno"); 
		if (setsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buffer, strlen(buffer)) != 0) {
			perror("setsockopt error"); 
			return -1;
		}
		socklen_t len = sizeof(buffer);
		if (getsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buffer, &(len)) != 0) {
			perror("getsockopt error"); 
			return -1; 
		} 
        printf("Algorithm changed to %s \n" , buffer);
    	
	    printf("connecting to server...\n");
    	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    	{ 
        	printf("connecting to server failed with error code : %d \n", errno); 
        	return -1; 
    	} 
        printf("connected...\n");	
		FILE *txt_file; 
		printf("Sending file... %d\n",i+1);
		txt_file = fopen("1gb.txt", "rb");
        if(txt_file == NULL){
        	perror("Error in reading file.");
        	return -1;
        }
        int data_chunk = 0;
        int Bytes_Sent = 0;
        int counter = 0;
        int Seg_size = 0;
    	while((data_chunk = fread(buffer, 1, sizeof(buffer), txt_file))>0 ){
	   		 Bytes_Sent = send(sock, buffer, data_chunk, 0);
			 if (Bytes_Sent == -1)
		     {
				printf("Sending file Error: %d", errno);
		     }
		     else if (Bytes_Sent == 0)
		     {
				printf("TCP connection closed before send() function.\n");
		     }
		     else if ( Bytes_Sent < data_chunk) 
		     {
				printf("Error: sent %d bytes from the %d required .\n", Bytes_Sent, data_chunk);
		     }
		     counter+=Bytes_Sent;
		     Seg_size+=1;
   		}
   		printf("Send %d bytes of file %d by %d segments.\n", counter, i+1, Seg_size);
		fclose(txt_file);
		close(sock);
	}

	return 0; 
} 
