#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
# define PORT 8081
# define BUFFER_SIZE 1024
int main () {
int server_fd , new_socket ;
struct sockaddr_in address ;
int addrlen = sizeof ( address ) ;
char buffer [ BUFFER_SIZE ];
// Create socket
server_fd = socket ( AF_INET , SOCK_STREAM , 0) ;
// Configure address
address . sin_family = AF_INET ;
address . sin_addr . s_addr = INADDR_ANY ;
address . sin_port = htons ( PORT ) ;
// Bind socket
// Listen for connections
// Accept connection
// Communication loop (to be implemented )
close ( new_socket ) ;
close ( server_fd ) ;
return 0;
}