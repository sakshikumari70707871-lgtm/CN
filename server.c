#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024

int count_words(char *str)
{
    int count = 0, in_word = 0;

    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
        {
            if (in_word == 0)
            {
                count++;
                in_word = 1;
            }
        }
        else
        {
            in_word = 0;
        }
    }

    return count;
}

int count_vowels(char *str)
{
    int count = 0;

    for (int i = 0; str[i] != '\0'; i++)
    {
        char c = str[i];

        if (c == 'a' || c == 'e' || c == 'i' ||
            c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' ||
            c == 'O' || c == 'U')
        {
            count++;
        }
    }

    return count;
}

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE];

    /* Create Socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    /* Initialize the value */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    /* Bind */
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    /* Listen */
    if (listen(server_fd, 5) < 0)
    {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    /* Accept */
    socklen_t addrlen = sizeof(address);

    new_socket = accept(server_fd,
                        (struct sockaddr *)&address,
                        &addrlen);

    if (new_socket < 0)
    {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");

    /* Handle multiple requests */
    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);

        int valread = read(new_socket, buffer, BUFFER_SIZE - 1);

        if (valread <= 0)
            break;

        buffer[strcspn(buffer, "\n")] = '\0';

        /* Client sends bye */
        if (strcmp(buffer, "bye") == 0)
            break;

        char command[20];
        char text[BUFFER_SIZE];

        command[0] = '\0';
        text[0] = '\0';

        /* Parse using | delimiter */
        sscanf(buffer, "%[^|]|%[^\n]", command, text);

        /* ANALYZE command */
        if (strcmp(command, "ANALYZE") == 0)
        {
            int chars = strlen(text);
            int words = count_words(text);
            int vowels = count_vowels(text);

            char response[BUFFER_SIZE];

            sprintf(response,
                    "Chars=%d,Words=%d,Vowels=%d",
                    chars, words, vowels);

            send(new_socket,
                 response,
                 strlen(response),
                 0);
        }
        else
        {
            char *msg = "Invalid Command";

            send(new_socket,
                 msg,
                 strlen(msg),
                 0);
        }
    }

    /* Close sockets */
    close(new_socket);
    close(server_fd);

    return 0;
}