#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include "tc.cpp"

//GLOBAL VARIABLES
int sockfd, portno, n;
struct sockaddr_in serv_addr;
struct hostent *server;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    
    int i;
    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    
    while(1)
    {
        bzero(buffer,256);
        printf("You: ");
        fgets(buffer,256,stdin);

        n = write(sockfd,buffer,strlen(buffer));

	if (n < 0) 
             error("ERROR writing to socket");

        if(strncmp("play()" , buffer , 6) == 0)
	{	printf("Game has started \n");
		int temp = play(sockfd);
		printf("Game has ended");      
        }
	else if(strncmp("playsnake()" , buffer , 11) == 0)
	{	printf("Game has started \n");
		int temp = playsnake(sockfd);
		printf("Game has ended\n");      
        }
 	else if(strncmp("Send()",buffer,6)==0)
	{
		
		printf("Initiating....\n");
		Send(sockfd);
		printf("You: \n");
	}
        
        bzero(buffer,256);
	printf("Wait for your friend...\n");
        n = read(sockfd,buffer,256);

	printf("Friend : %s\n",buffer);
        if (n < 0) 
             error("ERROR reading from socket");
	if(strncmp("play()" , buffer , 6) == 0)
	{	printf("Game has started \n");
		int temp = play(sockfd);
		printf("Game has ended");
		//printf("Wait for server to responde... \n");
        }
	else if(strncmp("playsnake()" , buffer ,11) == 0)
	{	printf("Game has started \n");
		int temp = playsnake(sockfd);
		printf("Game has ended");
		//printf("Wait for server to responde... \n");
        }
	else if(strncmp("Send()",buffer,6)==0)
	{
		
		printf("Initiating....\n");
		Recv(sockfd);
	}

        i = strncmp("Bye" , buffer , 3);
        if(i == 0)
               break;
	bzero(buffer,256);
    }
    close(sockfd);
    return 0;
}



