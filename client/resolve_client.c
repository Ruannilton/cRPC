#include "resolve_name.h"

#include "../rpc/rpc.h"
#define PORT "5000"
#define ADDRESS "127.0.0.1"

char *resolve_name(char *name)
{
    char buffer[1024];
    char *resolved = 0;
    size_t buffer_len = sizeof(buffer);
    SOCKET client_socket = rpc_connect_server(ADDRESS, PORT);

    if (client_socket != INVALID_SOCKET)
    {
        printf("Connected to server at %s:%s\n", ADDRESS, PORT);
        size_t stream_size = 0;
        size_t name_len = strlen(name);
        int size_readed = 0;

        char *payload_stream = create_payload_stream("resolve_name", name, name_len, &stream_size);

        if (send(client_socket, payload_stream, stream_size, 0) >= 0)
        {
            printf("Sent %d bytes\n", stream_size);

            if ((size_readed = recv(client_socket, buffer, buffer_len, 0)) > 0)
            {
                rpc_payload payload = parse_payload(buffer, size_readed);
                void *data = payload_get_data(&payload);
                if (payload_match_function(&payload, "resolve_name"))
                {
                    resolved = malloc(size_readed);
                    resolved = strcpy(resolved, data);
                }
            }
            else
            {
                printf("No response\n");
            }
        }

        free(payload_stream);
        rpc_disconnect_server(client_socket);
    }
    else
    {
        printf("Not connected\n");
    }
    return resolved;
}