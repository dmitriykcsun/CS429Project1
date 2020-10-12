#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>//to store address information
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<ifaddrs.h>
#include<linux/if_link.h>



int main(){

char server_message[256]="you have reached the server";         //holds the strings which will be sent to any clients we connect
char client_message[256];
//char tempString[256];

//IP address varibles
struct ifaddrs *ifaddr, *ifa;
int family, s;
socklen_t addrlen;
char host[NI_MAXHOST];

//create the server socket
int server_socket,client_socket;
int msgCounter = 0;
server_socket=socket(AF_INET,SOCK_STREAM,0);

//define the server address
struct sockaddr_in server_address;
server_address.sin_family=AF_INET;
server_address.sin_port=htons(9001);
server_address.sin_addr.s_addr=INADDR_ANY;                      //INADDR  any address in a local machine

//IP address procedure
if(getifaddrs(&ifaddr) == -1){
	perror("IP address failure");
	exit(1);
}

for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next){
	if(ifa->ifa_addr == NULL)
		continue;

	family = AF_INET;
	s = getnameinfo(ifa->ifa_addr, 
				(family == AF_INET) ? sizeof(struct sockaddr_in):
										sizeof(struct sockaddr_in6),
				host, NI_MAXHOST,
				NULL, 0, NI_NUMERICHOST);

}


//bind the socket to our specified IP and port
bind(server_socket,(struct sockaddr*) &server_address, sizeof(server_address));

listen(server_socket,5);                                        //5 could be any number that we want our connection waiting for a at time

fd_set mainServer,copyServer;

FD_ZERO(&mainServer);

FD_SET(server_socket, &mainServer);

memset(client_message,0,sizeof(client_message));


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
					if (strcmp(client_message, "myip\n") == 0){
						printf("IP address of server is: %s\n",host);
					}
					memset(client_message,0,sizeof(client_message));
					msgCounter++;
					if(msgCounter>5){
						close(server_socket);
						freeifaddrs(ifaddr);
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
freeifaddrs(ifaddr);
close(server_socket);
return 0;
}