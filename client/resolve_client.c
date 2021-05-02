#include "resolve_name.h"

#include "../rpc/rpc.h"

#define PORT "7000"
#define ADDRESS "127.0.0.1"

char *resolve_name(char *name)
{
    char buffer[64];
    char *resolved = 0;
    size_t buffer_len = sizeof(buffer);
    SOCKET client_socket = rpc_connect_server(ADDRESS, PORT);

    if (client_socket != INVALID_SOCKET)
    {
        size_t stream_size = 0;
        size_t name_len = strlen(name);
        int size_readed = 0;

        char *payload_stream = create_payload_stream("resolve_name", name, name_len, &stream_size);

        if (send(client_socket, payload_stream, stream_size, 0) >= 0)
        {
            if ((size_readed = recv(client_socket, buffer, buffer_len, 0)) > 0)
            {
                rpc_payload payload = parse_payload(buffer, size_readed);
                if (payload_match_function(&payload, "resolve_name"))
                {
                    void *data = payload_get_data(&payload);

                    resolved = malloc(payload.data_size + 1);
                    resolved = strncpy(resolved, data, payload.data_size);
                    resolved[payload.data_size] = '\0';
                }
            }
            else
            {
                return NULL;
            }
        }

        free(payload_stream);
        rpc_disconnect_server(client_socket);
    }
    else
    {
        return NULL;
    }
    return resolved;
}

int sum(int a, int b)
{
    char buffer[64];
    size_t buffer_len = 64;
    SOCKET client_socket = rpc_connect_server(ADDRESS, PORT);
    if (client_socket != INVALID_SOCKET)
    {
        size_t stream_size = 0;
        struct
        {
            int a;
            int b;
        } sum_params;
        sum_params.a = a;
        sum_params.b = b;

        char *payload_stream = create_payload_stream("sum", &sum_params, sizeof sum_params, &stream_size);

        if (send(client_socket, payload_stream, stream_size, 0) >= 0)
        {
            int size_readed = 0;
            if ((size_readed = recv(client_socket, buffer, buffer_len, 0)) > 0)
            {
                rpc_payload payload = parse_payload(buffer, size_readed);
                if (payload_match_function(&payload, "sum"))
                {
                    int data = *(int *)payload_get_data(&payload);
                    return data;
                }
            }
        }
    }
    return 0;
}