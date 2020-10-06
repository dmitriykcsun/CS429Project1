//# CS429Project1   client side of the socket
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>  //definition of the socket function we are using
#include<sys/socket.h>
#include<netinet/in.h>//to store address information  
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

int main(){
int msgCounter=0;
//create socket
int network_socket;                                   //integer to hold the socket descriptors
//create socket function
network_socket=socket(AF_INET,SOCK_STREAM,0);         //first param is the domain of the socket,second param is the type of the socket
                                                      //using default protocol 0 and streaming TCP for the third param
//specify an address for the socket
//we define the address in this section
struct sockaddr_in server_address;                    
server_address.sin_family=AF_INET;                    //sets family of the address
server_address.sin_port=htons(9001);                  //specify the port we are going to connect to
                                                      //hton function will take care of converting the port number(9001 or 9002 are the operating system port) 
                                                      //to be understood with our structure

server_address.sin_addr.s_addr=INADDR_ANY;            //connect to our local machine(s_addr is the sever address)
//connect returns an integer                          //s_addr is the real server we will be connecting to
int connection_status=connect(network_socket,(struct sockaddr *) &server_address,sizeof(server_address));       //cast out socket as the second parameters to
//check for error with the connection
if(connection_status==-1){
    printf("there was an error connecting to the remote socket");
}
//receive data from the server
char server_msg[256];
char client_msg[256];

while(1){
	printf("msg to send:");
	fgets(client_msg,256,stdin);
	send(network_socket,client_msg,strlen(client_msg),0);
	msgCounter++;
	if(msgCounter>5){
		close(network_socket);
		exit(0);
	}
}

//ecv(network_socket,& server_msg,sizeof(server_msg),0);    // the first param is the socket the second is a place to hlod the data to get back to
                                                           // and we pass the address of that string to our function, adn third is the size of that buffer
                                                           //and the last one is an optional flag which we make it to 0 
//print out the server's response
//printf("the server sent the data:%s\n",server_msg);
//close the socket
close(network_socket);

return 0;
}
