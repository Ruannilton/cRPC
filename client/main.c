#include "resolve_name.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    char *res = resolve_name(argv[1]);
    if (res)
    {
        printf("Resolve name: %s\n", res);
        free(res);
    }
    int s = sum(5, 6);
    printf("Sum: %d\n", s);
    int d;
    printf("Press to exit...");
    getchar();
    return 0;
}