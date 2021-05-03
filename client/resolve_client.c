#include "resolve_name.h"

#include "../rpc/rpc.h"

#define PORT "7000"
#define ADDRESS "127.0.0.1"

char *resolve_name(char *name)
{

    SOCKET client_socket = rpc_connect_server(ADDRESS, PORT);

    char *response = 0;
    int response_size = 0;
    if ((response_size = rpc_func_stub(client_socket, "resolve_name", name, (void **)(&response), strlen(name))) > 0)
    {
        response[response_size] = '\0';
        return (char *)response;
    }

    return NULL;
}

int sum(int a, int b)
{
    struct sum_params
    {
        int a;
        int b;
    } params = (struct sum_params){.a = a, .b = b};

    SOCKET client_socket = rpc_connect_server(ADDRESS, PORT);

    void *response = 0;
    if (rpc_func_stub(client_socket, "sum", (&params), &response, sizeof(struct sum_params)) > 0)
    {
        int sum_r = *(int *)response;
        free(response);
        return sum_r;
    }
    return 0;
}