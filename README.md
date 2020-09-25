//# CS429Project1
//server code
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

int getaddrinfo(const char *node,     // e.g. "www.example.com" or IP or host name to conncet to
                    const char *service,  // e.g. "http" or port number or a particular service
                    const struct addrinfo *hints,
                    struct addrinfo **res);
                    
/*Here’s a sample call if you’re a server who wants to listen on your host’s IP address, port 3490. 
Note that this doesn’t actually do any listening or network setup; it merely sets up structures we’ll use later:*/

int status;
struct addrinfo hints;
struct addrinfo *servinfo;  // will point to the results

memset(&hints, 0, sizeof hints); // make sure the struct is empty
hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

if ((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
}

// servinfo now points to a linked list of 1 or more struct addrinfos

// ... do everything until you don't need servinfo anymore ....

freeaddrinfo(servinfo); // free the linked-list
/*Here’s a sample call if you’re a client who wants to connect to a particular server, say “www.example.net” port 3490. Again, this doesn’t actually connect, but it sets up the structures we’ll use later:*/

int status;
struct addrinfo hints;
struct addrinfo *servinfo;  // will point to the results

memset(&hints, 0, sizeof hints); // make sure the struct is empty
hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
hints.ai_socktype = SOCK_STREAM; // TCP stream sockets

// get ready to connect
status = getaddrinfo("www.example.net", "3490", &hints, &servinfo);

// servinfo now points to a linked list of 1 or more struct addrinfos

// etc.

//Dmitry there is a cool program called show ip in Beej I ran it and showed the ip address of a www.... whatever you need try it!

int s;
struct addrinfo hints, *res;

// do the lookup
// [pretend we already filled out the "hints" struct]
getaddrinfo("www.example.com", "http", &hints, &res);

// again, you should do error-checking on getaddrinfo(), and walk
// the "res" linked list looking for valid entries instead of just
// assuming the first one is good (like many of these examples do).
// See the section on client/server for real examples.

s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

Here is the synopsis for the bind() system call:

    #include <sys/types.h>
    #include <sys/socket.h>
    
    int bind(int sockfd, struct sockaddr *my_addr, int addrlen);

//sockfd is the socket file descriptor returned by socket(). my_addr is a pointer to a struct sockaddr that contains information about your address, namely, port and IP //address. addrlen is the length in bytes of that address.

//an example that binds the socket to the host the program is running on, port 3490:

struct addrinfo hints, *res;
int sockfd;

// first, load up address structs with getaddrinfo():

memset(&hints, 0, sizeof hints);
hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

getaddrinfo(NULL, "3490", &hints, &res);

// make a socket:

sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

// bind it to the port we passed in to getaddrinfo():

bind(sockfd, res->ai_addr, res->ai_addrlen);

//The connect() call is as follows:

    #include <sys/types.h>
    #include <sys/socket.h>
    
    int connect(int sockfd, struct sockaddr *serv_addr, int addrlen); 

//an example where we make a socket connection to “www.example.com”, port 3490:

struct addrinfo hints, *res;
int sockfd;

// first, load up address structs with getaddrinfo():

memset(&hints, 0, sizeof hints);
hints.ai_family = AF_UNSPEC;
hints.ai_socktype = SOCK_STREAM;

getaddrinfo("www.example.com", "3490", &hints, &res);

// make a socket:

sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

// connect!

connect(sockfd, res->ai_addr, res->ai_addrlen);

 int listen(int sockfd, int backlog); 
 
 #include <sys/types.h>
    #include <sys/socket.h>
    
    int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen); 
    
    //how to use accept
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MYPORT "3490"  // the port users will be connecting to
#define BACKLOG 10     // how many pending connections queue will hold

int main(void)
{
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int sockfd, new_fd;

    // !! don't forget your error checking for these calls !!

    // first, load up address structs with getaddrinfo():

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    getaddrinfo(NULL, MYPORT, &hints, &res);

    // make a socket, bind it, and listen on it:

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bind(sockfd, res->ai_addr, res->ai_addrlen);
    listen(sockfd, BACKLOG);

    // now accept an incoming connection:

    addr_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);

    // ready to communicate on socket descriptor new_fd!
    .
    .
    .
int send(int sockfd, const void *msg, int len, int flags);

Some sample code might be:

char *msg = "Beej was here!";
int len, bytes_sent;
.
.
.
len = strlen(msg);
bytes_sent = send(sockfd, msg, len, 0);
.
int recv(int sockfd, void *buf, int len, int flags);

int sendto(int sockfd, const void *msg, int len, unsigned int flags,
               const struct sockaddr *to, socklen_t tolen); 
int recvfrom(int sockfd, void *buf, int len, unsigned int flags,
                 struct sockaddr *from, int *fromlen);              

close(sockfd);
//Just in case you want a little more control over how the socket closes, you can use the shutdown() function. It allows you to cut off communication in a certain direction, //or both ways (just like close() does). Synopsis:

    int shutdown(int sockfd, int how);

//The function getpeername() will tell you who is at the other end of a connected stream socket. The synopsis:

    #include <sys/socket.h>
    
    int getpeername(int sockfd, struct sockaddr *addr, int *addrlen);
    
//getpeername() is the function gethostname(). It returns the name of the computer that your program is running on

 #include <unistd.h>
    
    int gethostname(char *hostname, size_t size);
