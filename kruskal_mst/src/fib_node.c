#include <stdio.h>
#include <stdlib.h>

#include "../include/fib_node.h"

fib_node_t* create_fib_node(node_t* value) {
	fib_node_t* fib_node = (fib_node_t*)malloc(sizeof(fib_node_t));

	fib_node->node = value;
	fib_node->degree = 0;
	fib_node->mark = false;
	fib_node->left = fib_node->right = fib_node;
	fib_node->parent = fib_node->children = NULL;
	return fib_node;
}

void append(fib_node_t* list, fib_node_t* fib_node) {
	fib_node_t* head = list;
	fib_node_t* tail = list->left;

	tail->right = fib_node;
	fib_node_t* list2_tail = fib_node->left;
	fib_node->left = tail;
	tail = list2_tail;
	tail->right = head;
	head->left = tail;
}
