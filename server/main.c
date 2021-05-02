#include "../rpc/rpc.h"
#include "resolve_name.h"

void rpc_cllbck(rpc_server *serv, rpc_payload *payload, SOCKET client)
{
    char *data = payload_get_data(payload);

    if (payload_match_function(payload, "resolve_name"))
    {
        printf("Match: resolve_name\n");
        char *resolved = resolve_name(data);
        size_t response_size = 0;
        char *response = create_payload_stream("resolve_name", resolved, strlen(resolved), &response_size);
        if (send(client, response, response_size, 0) < 0)
        {
            printf("FAILED TO SEND DATA TO CLIENT\n");
        }
        free(resolved);
        free(response);
    }
    if (payload_match_function(payload, "sum"))
    {
        int a = *(int *)data;
        int b = *(int *)(data + sizeof(int));
        int res = sum(a, b);
        size_t response_size;
        char *response = create_payload_stream("sum", &res, sizeof(int), &response_size);
        if (send(client, response, response_size, 0) < 0)
        {
            printf("FAILED TO SEND DATA TO CLIENT\n");
        }
        free(response);
    }
}

int main()
{
    rpc_server server;
    rpc_server_start(&server, 128, 7000);
    printf("Server started!\n");
    rpc_server_run(&server, rpc_cllbck);
    rpc_server_close(&server);
    printf("Server closed\n");
    return 0;
}