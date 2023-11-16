#ifndef _FIB_NODE_H
#define _FIB_NODE_H

#include <stdbool.h>
#include <inttypes.h>

typedef struct node {
	int32_t value;
	bool mark;
	int16_t degree;
	struct node *left, *right;
	struct node *parent, *children;
} node_t;

node_t* create_node(int32_t value);
void append(node_t* list, node_t* node);
void print_forward(node_t* list);
void print_backward(node_t* list);

#endif
