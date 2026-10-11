/* UDP Client - Roll Number: 24559 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 30645

int main(int argc, char *argv[]) {
    int sock;
    char input[1024], msg[1100], reply[1024];
    struct sockaddr_in server;
    socklen_t len = sizeof(server);

    if (argc != 3) {
        printf("Usage: %s <server_ip> <roll>\n", argv[0]);
        return 1;
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if (inet_pton(AF_INET, argv[1],
                  &server.sin_addr) != 1) {
        printf("Invalid IP address\n");
        close(sock);
        return 1;
    }

    while (1) {
        printf("Enter command (UP|text, LOW|text, REV|text): ");

        if (!fgets(input, sizeof(input), stdin))
            strcpy(input, "EXIT");

        input[strcspn(input, "\n")] = '\0';

        snprintf(msg, sizeof(msg), "%s#%s", argv[2], input);

        if (sendto(sock, msg, strlen(msg), 0,
                   (struct sockaddr *)&server, len) < 0) {
            perror("sendto");
            break;
        }

        int n = recvfrom(sock, reply, sizeof(reply) - 1,
                         0, NULL, NULL);
        if (n < 0) {
            perror("recvfrom");
            break;
        }

        reply[n] = '\0';
        printf("Server response: %s\n", reply);

        if (strcmp(input, "EXIT") == 0)
            break;
    }

    close(sock);
    return 0;
}