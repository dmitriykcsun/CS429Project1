#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>//to store address information
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

int main(){

char server_message[256]="you have reached the server";         //holds the strings which will be sent to any clients we connect
char client_message[256];
//create the server socket
int server_socket,client_socket;
int msgCounter = 0;
server_socket=socket(AF_INET,SOCK_STREAM,0);

//define the server address
struct sockaddr_in server_address;
server_address.sin_family=AF_INET;
server_address.sin_port=htons(9001);
server_address.sin_addr.s_addr=INADDR_ANY;                      //INADDR  any address in a local machine

//bind the socket to our specified IP and port
bind(server_socket,(struct sockaddr*) &server_address, sizeof(server_address));

listen(server_socket,5);                                        //5 could be any number that we want our connection waiting for a at time

fd_set mainServer,copyServer;

FD_ZERO(&mainServer);

FD_SET(server_socket, &mainServer);

while(1)
{
	copyServer = mainServer;

	if(select(server_socket+1,&copyServer,NULL,NULL,NULL)<0)
	{
		perror("select failed\n");
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i<FD_SETSIZE; i++)
	{
		if(FD_ISSET(i,&copyServer))
		{
			if(i == server_socket)
			{
				client_socket = accept(server_socket,NULL,NULL);
				//send(client_socket,server_message,sizeof(server_message),0);
				while(recv(client_socket,client_message,256,0)){
					printf("%s",client_message);
					memset(client_message,0,sizeof(client_message));
					msgCounter++;
					if(msgCounter>5){
						close(server_socket);
						exit(0);
					}
				
				}
			}
		}
	}

}

//this integer holds the client socket
//int client_socket;

//client_socket=accept(server_socket,NULL,NULL);                    //first param the socket we are accepting the connections 
                                                                  //these NULL could be structures to tell us where the client is coming from
                                                                  //it contains the client address
//send message
//send(client_socket,server_message,sizeof(server_message),0);      //first param is the socket we are sending data on,second param is the actual data
                                                                  //because we already defined a server message will use that
close(server_socket);
return 0;
}