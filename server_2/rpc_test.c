#include "../rpc/rpc.h"
#include "resolve_name.h"

void rpc_cllbck(rpc_server *serv, rpc_payload *payload, SOCKET client)
{
    void *data = payload_get_data(payload);
    printf("Matching payload\n");
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
}

int main()
{
    rpc_server server;
    rpc_server_start(&server, 1024, 5000);
    printf("Server started!\n");
    rpc_server_run(&server, rpc_cllbck);
    rpc_server_close(&server);
    printf("Server closed\n");
    return 0;
}