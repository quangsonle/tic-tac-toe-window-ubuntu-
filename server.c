#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include<pthread.h>
#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
void timer_out(void);
char client_message[3];
char zmq_message[3];
char buffer[1024];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
void *context ;
void *requester;
char zmq_rec[10];
char i=0;
int socket_list[4];
clock_t start_t, end_t, total_t;///these variables are for timer

/* there is a problem of zmq that it if the structure is not
send-receive, it will report error (we could discuss this issue more)
therefore I implement the timer here. it is independent from the python judge, so if the timer is out and one wins (because another is so slow), we need to reset the judge also to restart the game
*/
void *  timer_check(void*arg)   // this is the thread handler for the timer
{
while(1)
{
end_t=clock();
if ((double)(end_t - start_t) / CLOCKS_PER_SEC>20) //check if after 20s still no new receive
{
	if (client_message[0]=='0')  // the last turn was X
	zmq_rec[0]=6; 
	if (client_message[0]=='1') // the last turn was X
	zmq_rec[0]=5;
	for (int k=0;k<2;k++)  // a proper send, broadcast the message to update both 2 clients (players)
		    send(socket_list[k],zmq_rec,12,0);
		    break;
}
}

}
void * socketThread(void *arg) /// here is the thread of the clients
{
  int newSocket = *((int *)arg);
  recv(newSocket , client_message , 2000 , 0);

  
  while(1)
  {
  
  int rec=recv(newSocket , client_message , 2 , 0); // receive from the client: 2 byte: the 1st is the ID (0 or 1), the second is the position they want to place (from 1-9 on the board)
  
  if( rec > 0)
    {
         
        
          if (i>=2) //check how many players, if less than 2, won't trigger the game
          {
          start_t=clock();
          strcpy(zmq_message,client_message);
          //printf("zmq send is is %s\n",zmq_message);// send zmq message to python
          zmq_send (requester, zmq_message, strlen(zmq_message), 0);
          zmq_recv (requester, zmq_rec, 10, 0);
         
          pthread_mutex_lock(&lock); //simple mutext to ensure no client can send while server is sending
          if ((int)zmq_rec[0]!=10) // ignore if if it is 10 (see python file)
	   { 
	   if (zmq_rec[0]=='w') // it is wrong order (see python file) so only feedback to the one that just sent
	   send(newSocket,zmq_rec,12,0);
	   else
	   {
	    for (int k=0;k<2;k++)  // a proper send, broadcast the message to update both 2 clients (players)
	    send(socket_list[k],zmq_rec,12,0);
	    
	    }
	    
	   }
	  
	   pthread_mutex_unlock(&lock);
          }
          else  // less than 2 player, woon't play game
          {
          send(socket_list[0],"ppp",3,0);
          printf("not enough player\n");
          }
    }
  
  }
}

int main(){
  int serverSocket, newSocket;
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

  //Create the socket. 
  serverSocket = socket(PF_INET, SOCK_STREAM, 0);

  // Configure settings of the server address struct
  // Address family = Internet 
  serverAddr.sin_family = AF_INET;

  //Set port number, using htons function to use proper byte order 
  serverAddr.sin_port = htons(5000);

  //Set IP address to localhost 
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");


  //Set all bits of the padding field to 0 
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  //Bind the address struct to the socket 
  bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  //Listen on the socket, with 40 max connection requests queued 
  if(listen(serverSocket,50)==0)
    printf("Listening\n");
  else
    printf("Error\n");
    pthread_t tid[2],timer_thread;
     i = 0;
    /////////////////////zmq config
   context = zmq_ctx_new ();
    requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://192.168.0.175:5555");
    ////////////////////
    pthread_create(&timer_thread, NULL, timer_check, NULL);
    while(1)
    {
    
        //Accept call creates a new socket for the incoming connection
        addr_size = sizeof serverStorage;
        
        newSocket= accept(serverSocket, (struct sockaddr *)  &serverStorage, &addr_size);//newSocket
       
        send(newSocket , &i , 1 , 0);// this send is for the client to know if he is eligible (if he get "i" is 2, which means we have enough players), i (the order they join the game) is also their ID, i don't use the IP as you mention because I run both on the same computer (only one loopback IP). For long term developmetn, using IP is not good because different network will grant different IP, even for the same machine
        sleep(0.1);
        socket_list[i]=newSocket;
        if( pthread_create(&tid[i++], NULL, socketThread, &newSocket) != 0 )// create the thread for the client, each client has its own thread
           printf("Failed to create thread\n");
    
        sleep(1);
    }
  return 0;
}
