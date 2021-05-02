#ifndef _RPC_H_
#define _RPC_H_

#include <stdbool.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#define PAYLOAD_DATA_STRIDE 2 * sizeof(size_t)
#define PAYLOAD_DESC_SIZE 4
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

typedef void (*rpc_callback)(rpc_server *, rpc_payload *, SOCKET);

int rpc_server_start(rpc_server *server, size_t buffer_len, int port);
void rpc_server_run(rpc_server *server, rpc_callback callback);
void rpc_server_close(rpc_server *server);

SOCKET rpc_connect_server(const char *address, const char *port);
void rpc_disconnect_server(SOCKET skt);

bool payload_match_function(rpc_payload *p, const char *fn_name);
void *payload_get_data(rpc_payload *p);
rpc_payload parse_payload(char *stream, size_t stream_size);
char *create_payload_stream(const char *fn_name, void *data, size_t data_size, size_t *out_stream_size);

void __attribute__((constructor)) inisock();
void __attribute__((destructor)) endsock();
void __stdcall freeaddrinfo(struct addrinfo *pAddrInfo);
INT __stdcall getaddrinfo(const char *pNodeName, const char *pServiceName, const struct addrinfo *pHints, struct addrinfo **ppResult);
#endif