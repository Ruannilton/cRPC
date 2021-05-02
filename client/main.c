#include "resolve_name.h"
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

int main(int argc, char *argv[])
{
    char *res = resolve_name(argv[1]);
    if (res)
    {
        printf("Resolve name: %s\n", res);
        free(res);
    }
    int d;
    printf("Press to exit...");
    getchar();
    return 0;
}