#ifndef _RPC_H_
#define _RPC_H_

#include <stdbool.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>

typedef struct rpc_payload rpc_payload;
struct rpc_payload
{
    size_t data_size, name_size;
    char *stream;
};

typedef struct rpc_server rpc_server;
struct rpc_server
{
    char *buffer;
    size_t buffer_len;
    struct sockaddr_in server_addr;
    SOCKET server_socket;
};

// typedef struct rpc_client rpc_client;
// struct rpc_client
// {
//     char *buffer;
//     size_t buffer_len;
//     SOCKET client_socket;
// };

typedef void (*rpc_callback)(rpc_server *, rpc_payload *, SOCKET);

int rpc_server_start(rpc_server *server, size_t buffer_len);
void rpc_server_run(rpc_server *server, rpc_callback callback);

SOCKET rpc_connect_server(const char *address, const char *port);

bool payload_match_function(rpc_payload *p, const char *fn_name);
void *payload_get_data(rpc_payload *p, char *stream);
rpc_payload parse_payload(char *stream, size_t stream_size);
char *create_payload_stream(const char *fn_name, void *data, size_t data_size);

void __attribute__((constructor)) inisock();
void __attribute__((destructor)) endsock();
void __stdcall freeaddrinfo(struct addrinfo *pAddrInfo);
INT __stdcall getaddrinfo(const char *pNodeName, const char *pServiceName, const struct addrinfo *pHints, struct addrinfo **ppResult);
#endif