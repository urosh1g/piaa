#ifndef _FIB_fib_node_H
#define _FIB_fib_node_H

#include <stdbool.h>
#include <inttypes.h>

#include "node.h"

typedef struct fib_node {
	node_t* node;
	bool mark;
	int16_t degree;
	struct fib_node *left, *right;
	struct fib_node *parent, *children;
} fib_node_t;

fib_node_t* create_fib_node(node_t* value);
void append(fib_node_t* list, fib_node_t* fib_node);

#endif
