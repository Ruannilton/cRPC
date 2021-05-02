#include "rpc.h"

bool payload_match_function(rpc_payload *p, const char *fn_name)
{
    char *name = malloc(p->name_size);
    strncpy(name, p->stream, p->name_size);
    bool same = false;
    if (strncmp(name, fn_name, strlen(fn_name)) == 0)
    {
        same = true;
    }
    free(name);
    return same;
}

void *payload_get_data(rpc_payload *p)
{
    return (void *)((size_t)p->stream + p->name_size);
}

rpc_payload parse_payload(char *stream, size_t stream_size)
{
    rpc_payload p;
    memcpy(&p.data_size, stream, PAYLOAD_DESC_SIZE);
    memcpy(&p.name_size, (void *)((size_t)stream + PAYLOAD_DESC_SIZE), PAYLOAD_DESC_SIZE);
    p.stream = (void *)(stream + PAYLOAD_DATA_STRIDE);
    return p;
}

char *create_payload_stream(const char *fn_name, void *data, size_t data_size, size_t *out_stream_size)
{
    //[data_size,name_len,name,data]

    size_t name_len = strlen(fn_name);
    size_t total_size = PAYLOAD_DATA_STRIDE + name_len + data_size;
    *out_stream_size = total_size;
    char *stream = malloc(total_size);

    memcpy(stream, &data_size, PAYLOAD_DESC_SIZE);
    memcpy((void *)((size_t)stream + PAYLOAD_DESC_SIZE), &name_len, PAYLOAD_DESC_SIZE);
    memcpy((void *)((size_t)stream + PAYLOAD_DATA_STRIDE), fn_name, name_len);
    memcpy((void *)((size_t)stream + PAYLOAD_DATA_STRIDE + name_len), data, data_size);
    return stream;
}

int rpc_server_start(rpc_server *server, size_t buffer_len, int port)
{

    server->server_addr = (struct sockaddr_in){
        .sin_family = AF_INET,
        .sin_addr.S_un = INADDR_ANY,
        .sin_port = htons(port)};

    server->server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (bind(server->server_socket, (struct sockaddr *)&(server->server_addr), sizeof server->server_addr) == SOCKET_ERROR)
    {
        return WSAGetLastError();
    }
    server->buffer = (char *)malloc(buffer_len);
    server->buffer_len = buffer_len;

    listen(server->server_socket, 5);
    return 0;
}

void rpc_server_run(rpc_server *server, rpc_callback callback)
{
    SOCKET server_socket = server->server_socket;
    SOCKET client_socket;
    struct sockaddr_in client_addr;
    int client_size;
    while (1)
    {
        printf("Waiting Connection ...\n");
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_size);
        while (client_socket == INVALID_SOCKET)
        {
            client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_size);
        }

        printf("Connection found!!\n");
        memset(server->buffer, '\0', server->buffer_len);

        int readed_size = recv(client_socket, server->buffer, server->buffer_len, 0);
        if (readed_size != SOCKET_ERROR)
        {
            printf("Receive %d bytes\n", readed_size);
            rpc_payload payload = parse_payload(server->buffer, readed_size);
            callback(server, &payload, client_socket);
        }
        else
        {
            printf("ERROR [%d]: No data received\n", WSAGetLastError());
        }

        closesocket(client_socket);
    }
}

void rpc_server_close(rpc_server *server)
{
    free(server->buffer);
    closesocket(server->server_socket);
}

SOCKET rpc_connect_server(const char *address, const char *port)
{
    struct addrinfo *result = 0, *ptr = 0, hints;
    ZeroMemory(&hints, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(address, port, &hints, &result) != 0)
    {
        printf("ERROR [%d]: rpc_connect_server\n", WSAGetLastError());
        return INVALID_SOCKET;
    }

    SOCKET client_socket = INVALID_SOCKET;

    for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
    {
        client_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        if (client_socket == INVALID_SOCKET)
        {
            return INVALID_SOCKET;
        }

        if ((connect(client_socket, ptr->ai_addr, (int)ptr->ai_addrlen)) == SOCKET_ERROR)
        {
            closesocket(client_socket);
            client_socket = INVALID_SOCKET;
            continue;
        }

        break;
    }

    if (client_socket == INVALID_SOCKET)
    {
        printf("ERROR [%d]: rpc_connect_server\n", WSAGetLastError());
        return -1;
    }

    freeaddrinfo(result);
    return client_socket;
}

void rpc_disconnect_server(SOCKET skt)
{
    shutdown(skt, SD_SEND);
    closesocket(skt);
}

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