#include <stdio.h>
#include <stdlib.h>

#include "../include/node.h"

node_t* create_node(int32_t value) {
	node_t* node = (node_t*)malloc(sizeof(node_t));

	node->value = value;
	node->degree = 0;
	node->mark = false;
	node->left = node->right = node;
	node->parent = node->children = NULL;
	return node;
}

void append(node_t* list, node_t* node) {
	node_t* head = list;
	node_t* tail = list->left;

	tail->right = node;
	node_t* list2_tail = node->left;
	node->left = tail;
	tail = list2_tail;
	tail->right = head;
	head->left = tail;
}

void print_forward(node_t* list) {
	node_t* iter = list;
	do {
		printf("%"PRId32" ", iter->value);
		iter = iter->right;
	} while(iter != list);
	printf("\n");
}
void print_backward(node_t* list) {
	node_t* iter = list;
	do {
		printf("%"PRId32" ", iter->value);
		iter = iter->left;
	} while(iter != list);
	printf("\n");
}
