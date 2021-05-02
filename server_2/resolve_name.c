#include "resolve_name.h"

char *resolve_name(char *name)
{
    char tmp_buffer[32];
    char *token = strtok(name, " ");
    size_t count = 0;
    while (token)
    {
        tmp_buffer[count] = token[0];
        token = strtok(NULL, " ");
        count++;
    }
    tmp_buffer[count] = '\0';
    count++;
    char *resolved = malloc(count * sizeof(char));
    resolved = strcpy(resolved, tmp_buffer);
    return resolved;
}