#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include<pthread.h>
void print_board(char recb[12])  // to print the board
{


if ((int)recb[0]!=0)  // ignore if the first element is 0 (invalid)
{
if (recb[0]=='w') // if the 1st element is 'w", warn player this is not his turn (see server and python files)
{
printf("wait!!!! don't press, not your turn\n");
return;
}
if (recb[0]=='p')  // not enough player, ask them to wait (see server file)
{
printf("please wait for another player\n");
return;
}
// from here: to print the board
printf("%c|%c|%c\n",recb[7],recb[8],recb[9]);
printf("-+-+-\n");
printf("%c|%c|%c\n",recb[4],recb[5],recb[6]);
printf("-+-+-\n");
printf("%c|%c|%c\n",recb[1],recb[2],recb[3]);
//from here: to print the result
switch ((int)recb[0])
{
case 3:
printf("X won\n");
break;
case 4:
printf("O won\n");
break;
case 5:
printf("X won because time out\n");
break;
case 6:
printf("O won because time out\n");
break;
}

}
return;

}
void * recthread(void *arg)  // a thread for receive for the server (indepedent from main thread that needs to read input characters)
{
char rec_buffer[10];
int Socket = *((int *)arg);
while(1)
{
if(recv(Socket, rec_buffer, 50, 0) < 0)
    {
       printf("Receive failed\n");
       return 0;
    }
    
    print_board(rec_buffer);
}
}
int main(){
  int i = 0;
 // pthread_t tid;
  //////
  char message[3];
  char buffer;
  
  int clientSocket;
  struct sockaddr_in serverAddr;
  socklen_t addr_size;

  // Create the socket. 
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);

  //Configure settings of the server address
 // Address family is Internet 
  serverAddr.sin_family = AF_INET;

  //Set port number, using htons function 
  serverAddr.sin_port = htons(5000);

 //Set IP address to localhost
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // loopback ip, no need to change
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    //Connect the socket to the server using the address
    addr_size = sizeof serverAddr;
    int error =connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
  if ( error!=0)
    
    {
    printf(" error\n");
    return 0;
    }
    
    recv(clientSocket, &buffer, 1, 0);
    if (buffer>=2)
    {
    printf("enough 2 players, please wait for the next game\n");
    close(clientSocket);
    return 0;}
    
  if (   send(clientSocket , "OK" , strlen("OK") , 0)<0)
  {
 printf(" error\n");
   return 0;
    }
    pthread_t tid;
    pthread_create(&tid, NULL, recthread,&clientSocket);
  /////
  printf("welcome to the game, ensure your rival also joined\n");
  printf(" press a button (only number from 1-9) to choose the place on the board you want to play, according to the below board\n");// note here i dont detect if user presses non-number button, it could crash the program
  print_board((char[10]){1,'1','2','3','4','5','6','7','8','9'});
  
    while(1)
{
int c = getchar();



sprintf(message, "%c%c", buffer+48,c);// send 2 characters, first is the id, second is is the button (number) that user presses
//printf("c sdad  %s",message);
   if( send(clientSocket , message , strlen(message) , 0) < 0)
    {
            printf("Send failed\n");
    }

  
}
return 0;
}
