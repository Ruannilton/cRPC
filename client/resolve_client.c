#include "resolve_name.h"

#include "../rpc/rpc.h"

#define PORT "7000"
#define ADDRESS "127.0.0.1"

char *resolve_name(char *name)
{

    SOCKET client_socket = rpc_connect_server(ADDRESS, PORT);

    if (client_socket != INVALID_SOCKET)
    {

        if (rpc_client_send(client_socket, "resolve_name", name, strlen(name)) > 0)
        {
            rpc_payload response;
            if (rpc_client_read(client_socket, "resolve_name", &response, 64) > 0)
            {
                void *data = payload_get_data(&response);
                char *resolved = malloc(response.data_size + 1);
                resolved = strncpy(resolved, data, response.data_size);
                resolved[response.data_size] = '\0';
                free_payload(&response);
                rpc_disconnect_server(client_socket);
                return resolved;
            }
        }
        rpc_disconnect_server(client_socket);
    }

    return NULL;
}

int sum(int a, int b)
{
    SOCKET client_socket = rpc_connect_server(ADDRESS, PORT);
    if (client_socket != INVALID_SOCKET)
    {
        struct sum_params
        {
            int a;
            int b;
        } params = (struct sum_params){.a = a, .b = b};

        if (rpc_client_send(client_socket, "sum", (char *)(&params), sizeof(struct sum_params)) > 0)
        {
            rpc_payload response;
            if (rpc_client_read(client_socket, "sum", &response, sizeof(int)) > 0)
            {
                int data = *(int *)payload_get_data(&response);
                free_payload(&response);
                rpc_disconnect_server(client_socket);
                return data;
            }
        }
        rpc_disconnect_server(client_socket);
    }
    return 0;
}