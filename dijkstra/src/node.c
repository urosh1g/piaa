#include <stdio.h>
#include <stdlib.h>

#include "../include/node.h"
#include "../include/edge.h"

node_t* create_node(int value, node_t* next) {
	node_t* node = (node_t*)malloc(sizeof(node_t));

	node->dist   = 0;
	node->adj    = NULL;
	node->_dprev = NULL;
	node->next   = next;
	node->value  = value;
	node->status = NOTVISITED;
	return node;
}
