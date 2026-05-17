#ifndef RESOURCE_H
#define RESOURCE_H

#include "list.h"

typedef struct resource {
    int ceiling;
    list_node_t node;
    list_t wait_queue;
} resource_t;

void resource_init(resource_t *res, int ceiling);
void resource_get(resource_t *res);
void resource_release(resource_t *res);

#endif