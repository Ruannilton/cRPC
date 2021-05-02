#include "resolve_name.h"

#include <winsock2.h>

#define PORT 5000
#define ADDRESS "127.0.0.1"

int main()
{
    struct WSAData wsa;
    WSAStartup(MAKEWORD(2, 0), &wsa);

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_addr.S_un = INADDR_ANY,
        .sin_port = htons(PORT)};

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof server_addr) == SOCKET_ERROR)
    {
        printf("[%d] Failed to bind socket\n", WSAGetLastError());
    }
    listen(server_socket, 5);

    SOCKET client_socket;
    struct sockaddr_in client_addr;
    int client_size;
    char buffer[257];
    while (1)
    {
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_size)) != INVALID_SOCKET)
        {
            printf("Socket Connected\n");

            int readed_size = recv(client_socket, buffer, 256 * sizeof(char), 0);
            if (readed_size > 0)
            {
                char *name_resolved = resolve_name(buffer);
                buffer[256] = '\0';
                printf("READED %d BYTES FROM CLIENT: %s\n", readed_size, buffer);
                printf("SENT TO CLIENT: %s\n", name_resolved);
                size_t str_len = strlen(name_resolved);
                if (send(client_socket, name_resolved, str_len, 0) < 0)
                {
                    printf("FAILED TO SEND DATA TO CLIENT\n");
                }
                free(name_resolved);
            }
            closesocket(client_socket);
            printf("Socket Closed\n");
        }
    }

    closesocket(server_socket);
    WSACleanup();

    return 0;
}