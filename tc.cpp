#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <iostream>
#include "filehandling.cpp"
using namespace std;

char c[3][3];
int arr[3][3]={
	{1,2,3},
	{4,5,6},
	{7,8,9}
};
int arr2[10][10] = {
			{100,99,98,97,96,95,94,93,92,91},
			{11,12,13,14,15,16,17,18,19,20},
			{21,22,23,24,25,26,27,28,29,30},
			{31,32,33,34,35,36,37,38,39,40},
			{41,42,43,44,45,46,47,48,49,50},
			{51,52,53,54,55,56,57,58,59,60},	
			{61,62,63,64,65,66,67,68,69,70},
			{71,72,73,74,75,76,77,78,79,80},
			{81,82,83,84,85,86,87,88,89,90},
			{91,92,93,94,95,96,97,98,99,100}
		    };



int q[]={4,22,44,62,70,33,47,58,78,99};  //first 5 are location of snaes and neaxt 5 are location of snakes
int d[]={15,33,22,20,21,-10,-22,-10,-22,-56}; //jump values in case of snake or ladder

int playsnake(int sockfd)
{

	int a=0,b=0,p=0,e=0,f,x,y=0,i;
	char n;
	int buf;
	
	printf("\nYour friend is first, wait for responce \n");
    while(1)
   {
		
		//SERVER WILL PLAY FIRST
	n = read(sockfd, &buf, sizeof(buf));      
	b= buf;
	
	time_t t;		//initializing seeding random values
	t=time(NULL);
	srand(t);
	while(1)				//rolling dice
  	{
		printboard();
  		cout<<"\nPress any Enter/Return to roll the dice";
		scanf("%c",&n);
  		if (n==10)		// 10 is the ASCII code of return/enter or line break \n
    		{
    		x=(rand()%6)+1;
    		break;
    		}
  	}

	cout<<"The dice tuned to be :"<<x<<endl;//rollind dice
	
	while(1)		// case when dice turns to be 6
	  {
  		if (x!=6 && y!=6)
 		 break;
 		 while(1)		// loop condition to roll the dice
   		 {
   		 printf("Your dice returned to be 6. So, \n");
   		 printf("\nPress Enter/Return to roll the dice again");
    		 scanf("%c",&n);		// 10 is the ASCII code for return/enter or line break \n
    			 if (n==10)
      		 	 {
     		 	 y=(rand()%6)+1;
     		 	 break;
     			 }
   		  }	
	 printf("The new dice value is %d\n",y);
 	 x=x+y;
  	 }	
	
	p=a;
	if (p+x > 100)
  	cout<<"The Value exceeds 100 so the player will not move\n";
	else if (p+x <= 100)
  	{
  	p=p+x;			//increment of player position according to the return value from dice roll
  		for(i=0;i<10;i++)		//condition to check snakes and ladders
    		{
    		if (p==q[i])
    		p=p+d[i];
    		}
  	}	
	
	a=p;			//geting back the value of a from a dummy value
  	printf("Client have reached the box value = %d\n",a);
	buf=a;
	n = write( sockfd, &buf , sizeof(buf) );
	printf("\nServer have reached the box value = %d\n\n",b);

	if (a==100)
  	{
  	cout<<"\nPlayer on client side wins the game\n";	//case when player 1 wins
  	break;
  	}
	if (b==100)
  	{
  	cout<<"\nPlayer on server side wins the game\n";		//case when player 2 wins
  	break;
  	}
   }
	return 1;
	
}

//FOR PRINTING MATRIX
void print(char a[3][3])
{
	int i,j,k;
	for( i = 0; i<3 ; i++)
 	{
		for(j = 0; j<3; j++)
		{	if(a[i][j]=='\0')
				printf("   ");	
			else
				printf(" %c ",a[i][j]);
			if( j != 2)
				printf("|");					
		}
		printf("\n");
		if( i != 2)
			printf("-----------\n");
	}
	printf("\n");
}

//FOR CHECKING X AND O
void checkXO(int a, int d, char c[3][3])
{
    if (c[a][d] == 'O')
        printf("\n******************YOU WON!!******************\n");
    else
        printf("\n******************YOU LOST!!******************\n");
}

//FOR CHECKING IF SOMEONE IS WON
int check(){
    int n;
    for( n=0;n<3;n++)
    {
        if((c[0][n]==c[1][n]) && (c[0][n]==c[2][n]) && (c[0][n]!='\0'))
            {checkXO(0,n,c);return 1;}
        else if( (c[n][1]==c[n][0]) && (c[n][1]==c[n][2]) && (c[n][1]!='\0'))
            {checkXO(n,0,c);return 1;}
    }
	if((c[0][0]==c[1][1]) && (c[1][1]==c[2][2]) && (c[1][1]!='\0'))
            {checkXO(0,0,c);return 1;}
        else if((c[0][2]==c[1][1]) && (c[1][1]==c[2][0]) && (c[1][1]!='\0'))
            {checkXO(0,2,c);return 1;}

    return 0;
}

int play(int sockfd)
{
	int n, buffer;
	int i,j;
	printf("*********Instructions*********\nType the number to print X on that position \n");
	//print((char)arr);

	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			c[i][j] = '\0';
		}
	}
	printf("Your friend is first, wait for responce \n");
	while(1)
	{
		
		//SERVER WILL PLAY FIRST
		n = read(sockfd, &buffer, sizeof(buffer));      
		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				if(arr[i][j]==buffer)
					c[i][j] = 'X';
			}
		}
		print(c);
		if(check()==1)
		{
			return 0;
		}
		int temp = 0;
		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				if(c[i][j] == 'X' || c[i][j] == 'O')
					temp++;
			}
		}		
		if(temp==9){
			printf("\n******************Draw******************\n");
		}
		printf("YOUR TURN \n");
		printf("Enter cell number-> ");
		scanf("%d",&buffer);
		n = write( sockfd, &buffer , sizeof(buffer) );

		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				if(arr[i][j]==buffer)
					c[i][j] = 'O';
			}
		}
		if(check()==1)
		{
			return 0;
		}
		print(c);
	}

	return 1;
	
}

void Send(int sockfd)
{
	char buffer[512];
	bzero(buffer,512);
        
    	FILE *f;
    
    int words = 0;
    char c;
	char fn[20];
	Q:
	printf("Enter the file name \n");
	scanf("%s",fn);
     f=fopen(fn,"r");
	if(f == NULL){
		printf("Error no file found");
		goto Q;
	}
    while((c=getc(f))!=EOF)			//Counting No of words in the file
	{	
		fscanf(f , "%s" , buffer);
		if(isspace(c)||c=='\t')
		words++;	
	}
       
      
	write(sockfd, &words, sizeof(int));
     	rewind(f);
       
    char ch ;
       while(ch != EOF)
      {
		
		fscanf(f , "%s" , buffer);
		write(sockfd,buffer,512);
		ch = fgetc(f);
      }
	printf("The file was sent successfully");
	bzero(buffer,512);
}

void Recv(int newsockfd)
{
	          FILE *fp;
         int ch = 1;
	char buffer[512];
            fp = fopen("recieved.txt","a");
            int words;
		read(newsockfd, &words, sizeof(int));
          while(ch != words)
       	   {
        	 read(newsockfd , buffer , 512);
	   	 fprintf(fp , " %s" , buffer);
		 
		 ch++;
	   }
     	printf("The file was received successfully\n");
	   printf("The new file created ");
	bzero(buffer,512);
	   fclose(fp);
}
