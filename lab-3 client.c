#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
# define PORT 8081
# define BUFFER_SIZE 1024
int main () {
int sock ;
struct sockaddr_in serv_addr ;
char buffer [ BUFFER_SIZE ];
// Create socket
sock = socket ( AF_INET , SOCK_STREAM , 0) ;
// Configure server address
serv_addr . sin_family = AF_INET ;
serv_addr . sin_port = htons ( PORT ) ;
// Convert IP address (127.0.0.1)
// Connect to server
// Communication loop (to be implemented )
close ( sock ) ;
return 0;
}