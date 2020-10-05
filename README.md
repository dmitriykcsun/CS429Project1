//# CS429Project1   client side of the socket
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>  //definition of the socket function we are using
#include<sys/socket.h>
#include<netinet/in.h>//to store address information  

int main(){
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
char server_response[256];
recv(network_socket,& server_response,sizeof(server_response),0);    // the first param is the socket the second is a place to hlod the data to get back to
                                                                     // and we pass the address of that string to our function, adn third is the size of that buffer
                                                                     //and the last one is an optional flag which we make it to 0 
//print out the server's response
printf("the server sent the data a:%s\n",server_response);
//close the socket
close(network_socket);
network_socket.close();

return 0;
}

//server side of the socket
//we need another file for this part

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>//to store address information

int main(){

char server_message[256]="you have reached the server";         //holds the strings which will be sent to any clients we connect
//create the server socket
int server_socket;
server_socket=socket(AF_INET,SOCK_STREAM,0);
//this integer holds the client socket
int client_socket;
//define the server address
struct sockaddr_in server_address;
server_address.sin_family=AF_INET;
server_address.sin_port=htons(9001);
server_address.sin_addr.s_addr=INADDR_ANY;                      //INADDR  any address in a local machine

//bind the socket to our specified IP and port
bind(server_socket,(struct sockaddr*) &server_address, sizeof(server_address));

listen(server_socket,5);                                      //5 could be any number that we want our connection waiting for a at time

fd_set mainServer,copyServer;

FD_ZERO(&mainServer);

FD_SET(server_socket, &mainServer);
int msgCounter=0;
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
				send(client_socket,server_message,sizeof(server_message),0);
				msgCounter++;
				if(msgCounter>5){
					close(server_socket);
					exit(0);
				}
			}
		}
	}

}



client_socket=accept(server_socket,NULL,NULL);                    //first param the socket we are accepting the connections 
                                                                  //these NULL could be structures to tell us where the client is coming from
                                                                  //it contains the client address
 //send message
send(client_socket,server_message,sizeof(server_message),0);      //first param is the socket we are sending data on,second param is the actual data
                                                                  //because we already defined a server message will use that
close(server_socket);
return 0;
}

//by running these we should have a two way connection between client and server
//as the insrtuction says we can run these with two terminal windows
//I am not sure if I know how to do that but it seems simple
// put the screen in two parts
//on left run the server by: make tcp_server (I am not sure if this is the same as compiling with gcc?
//on right run the client by: make tcp_client
//first start with tcp_server type: ./tcp_server
//then in client part : ./tcp_client 
then you will see the a message in client side



new loop which makes our program work like a chatroom
//listening is server_socket
//master is mainServer

while(true){

  fd_set copy=mainServer; //in our code mainServer
  int socketCount=select(0,&copy,NULL,NULL,NULL);
  for(int i=0;i<socketCount;i++){
    socket sock=Copy.fd_array[i];
    if(sock==listening){
        //accept a new connection 
        SOCKET client=accept(listening,NULL,NULL,NULL);
        //add the new connection to the list of connected clients
        FD_SET(client, &mainServer);
        //send welcom message the the connected client
        string welcomeMsg="welcome to the awsome chat server";
        send(client,welcome.c_str(),welcomMsg.size()+1,0)
    }//if
    else{
          charbuf[4096];
          zeroMemory(buf,4096);
          //receiving message
          int bytesIn=rec(Sock,buf,4096,0);
          if(bytesIn<=0){
            //drop the client
            closeSocket(sock);
            FD_CLR(sock,&mainServer);
          }//if
          else{
            //send message to other clients and not definately to the listening socket
            for(int i=0;i<master.fd_count;i++){
              Socket outSock=mainServer.fd_array[i];
              if(outSock!=listening && outSock!=Sock){
                 //this part is so c++
                 //this part shows some client id
                 ostringStream ss;
                 ss<<"socket #"<<sock<<":"<<buf<<"\r\n;//I have not worked with strings in c. how can we change it to c?
                 string strout=ss.str();
                 send(outSock,strOut.c_str(),strOut.size()+1,0);
              }//if
            }//for
          }//else
    }//else
  }//for
}//while  


