#ifndef _FIB_HEAP_H
#define _FIB_HEAP_H

#include <stdio.h>
#include <stddef.h>
#include "../include/fib_node.h"

typedef struct fib_heap {
	fib_node_t* min;
	size_t nodes;
	size_t t, m;
} fib_heap_t;

fib_heap_t* create_fib_heap();
fib_node_t* fib_heap_insert(fib_heap_t* heap, node_t* value);
node_t* fib_heap_extract_min(fib_heap_t* heap);
void fib_heap_decrease_key(fib_heap_t* heap,
							node_t* node,
							int32_t new_value);
void fib_heap_delete_fib_node(fib_heap_t* heap, node_t* node);
void fib_heap_destroy(fib_heap_t* heap);

#endif
