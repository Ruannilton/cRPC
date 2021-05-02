#include "resolve_name.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT "5000"
#define ADDRESS "127.0.0.1"

void inisock()
{
    struct WSAData wsa;
    if (WSAStartup(MAKEWORD(2, 0), &wsa) != 0)
    {
        printf("Failed to start\n");
    }
}

void endsock()
{
    WSACleanup();
}

void __stdcall freeaddrinfo(struct addrinfo *pAddrInfo);
INT __stdcall getaddrinfo(const char *pNodeName, const char *pServiceName, const struct addrinfo *pHints, struct addrinfo **ppResult);

char *resolve_name(char *name)
{
    struct addrinfo *result = 0, *ptr = 0, hints;
    ZeroMemory(&hints, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(ADDRESS, PORT, &hints, &result) != 0)
    {
        printf("[%d] Failed to get addrinfo\n", WSAGetLastError());
    }

    char buffer[257];
    char *resolved = 0;

    SOCKET client_socket = INVALID_SOCKET;

    for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
    {
        client_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        if (client_socket == INVALID_SOCKET)
        {
            printf("[%d] Failed to create socket\n", WSAGetLastError());
            return 0;
        }

        if ((connect(client_socket, ptr->ai_addr, (int)ptr->ai_addrlen)) == SOCKET_ERROR)
        {
            closesocket(client_socket);
            client_socket = INVALID_SOCKET;
            continue;
        }

        break;
    }

    freeaddrinfo(result);

    if (send(client_socket, name, strlen(name), 0) >= 0)
    {
        int size_readed = 0;
        if ((size_readed = recv(client_socket, buffer, 256 * sizeof(char), 0)) > 0)
        {
            buffer[256] = '\0';
            if (size_readed > 0)
            {

                resolved = malloc(size_readed);
                resolved = strcpy(resolved, buffer);
            }
            else
            {
                printf("No response\n");
            }
        }
        else
        {
            printf("No response\n");
        }
    }
    shutdown(client_socket, SD_SEND);
    closesocket(client_socket);
    return resolved;
}