#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];
    char input[BUFFER_SIZE];

    // Create Socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Server IP address
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        close(sock);
        return 1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr,
                sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        return 1;
    }

    printf("Connected to server.\n");

    // Take input from user
    printf("Enter text: ");
    fgets(input, BUFFER_SIZE, stdin);

    // Send input to server
    char message[BUFFER_SIZE + 20];

snprintf(message,sizeof(message), "ANALYZE|%s", input);

send(sock, message, strlen(message), 0);

    // Receive response from server
    int n = recv(sock, buffer, BUFFER_SIZE - 1, 0);

    if (n > 0) {
        buffer[n] = '\0';
        printf("Server Response: %s\n", buffer);
    } else {
        printf("No response from server.\n");
    }

    // Close socket
    close(sock);

    return 0;
}