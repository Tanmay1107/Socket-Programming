/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "ts.cpp"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,&clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     while(1)
     {
		printf("Wait for your friend...\n");
           bzero(buffer,256);
           n = read(newsockfd,buffer,256);
           if (n < 0) 
		error("ERROR reading from socket");
           printf("Friend : %s\n",buffer);
	if(strncmp("play()",buffer,6)==0)
	{
		printf("Game has started\n");
		int temp = play(newsockfd);
		printf("Game has ended\n");
		printf("You: ");
	}
	else if(strncmp("playsnake()",buffer,11)==0)
	{
		printf("Game has started\n");
		int temp = playsnake(newsockfd);
		printf("Game has ended\n");
		printf("You: ");
	}
	else if(strncmp("Send()",buffer,6)==0)
	{
		
		printf("Initiating....\n");
		Recv(newsockfd);
		printf("You: \n");
	}
          bzero(buffer,256);
	printf("You: ");
          fgets(buffer,256,stdin);
          n = write(newsockfd,buffer,strlen(buffer));
	if (n < 0) error("ERROR writing to socket");
	if(strncmp("play()",buffer,6)==0)
	{
		printf("Game has started\n");
		int temp = play(newsockfd);
		printf("Game has ended\n");
	}
	else if(strncmp("playsnake()",buffer,11)==0)
	{
		printf("Game has started\n");
		int temp = playsnake(newsockfd);
		printf("Game has ended\n");
	}
	else if(strncmp("Send()",buffer,6)==0)
	{
		
		printf("Initiating....\n");
		Send(newsockfd);
	}
           int i=strncmp("Bye" , buffer, 3);
           if(i == 0)
               break;
	bzero(buffer,256);
     }
     close(newsockfd);
     close(sockfd);
     return 0; 
}
