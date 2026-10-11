/* Roll: 24559 Service Type: A d = 0 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>

#define ROLL "24559"
#define PORT 30645

int main() {
    int sock;
    char buf[1024], reply[1024];
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(sock, (struct sockaddr *)&server,
             sizeof(server)) < 0) {
        perror("bind");
        close(sock);
        return 1;
    }

    printf("Server running on port %d\n", PORT);

    while (1) {
        int n = recvfrom(sock, buf, sizeof(buf) - 1, 0,
                         (struct sockaddr *)&client, &len);
        if (n < 0) continue;
        buf[n] = '\0';

        printf("\nClient: %s:%d\n",
               inet_ntoa(client.sin_addr),
               ntohs(client.sin_port));

        char *p = strchr(buf, '#');

        if (!p || (size_t)(p - buf) != strlen(ROLL) ||
            strncmp(buf, ROLL, strlen(ROLL)) != 0) {
            strcpy(reply, "ERROR: Unauthorized client");
        } else {
            char msg[1024];
            strcpy(msg, p + 1);
            printf("Received: %s\n", msg);

            if (strcmp(msg, "EXIT") == 0) {
                strcpy(reply, "Server shutting down");
                sendto(sock, reply, strlen(reply), 0,
                       (struct sockaddr *)&client, len);
                printf("Sent: %s\n", reply);
                break;
            }

            char *sep = strchr(msg, '|');

            if (!sep) {
                strcpy(reply, "ERROR: Invalid command");
            } else {
                *sep = '\0';
                char *text = sep + 1;

                if (strcmp(msg, "UP") == 0) {
                    for (int i = 0; text[i]; i++)
                        text[i] = toupper((unsigned char)text[i]);
                    strcpy(reply, text);
                } else if (strcmp(msg, "LOW") == 0) {
                    for (int i = 0; text[i]; i++)
                        text[i] = tolower((unsigned char)text[i]);
                    strcpy(reply, text);
                } else if (strcmp(msg, "REV") == 0) {
                    int i = 0, j = strlen(text);
                    if (j > 0) j--;
                    while (i < j) {
                        char temp = text[i];
                        text[i++] = text[j];
                        text[j--] = temp;
                    }
                    strcpy(reply, text);
                } else {
                    strcpy(reply, "ERROR: Invalid command");
                }
            }
        }

        sendto(sock, reply, strlen(reply), 0,
               (struct sockaddr *)&client, len);
        printf("Sent: %s\n", reply);
    }

    close(sock);
    return 0;
}