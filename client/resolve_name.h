#ifndef __RESOLVE_NAME_H_
#define __RESOLVE_NAME_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void __attribute__((constructor)) inisock();
void __attribute__((destructor)) endsock();

char *resolve_name(char *name);

#endif