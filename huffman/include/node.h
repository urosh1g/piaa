#ifndef NODE_H
#define NODE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct node {
	uint8_t ch;
	size_t freq;
	struct node *left, *right;
	char* code;
	size_t code_len;
} node_t;

node_t* node(uint8_t ch, size_t freq, node_t* left, node_t* right){
	node_t* n = (node_t*)malloc(sizeof(node_t));
	n->ch = ch;
	n->freq = freq;
	n->left = left;
	n->right = right;
	return n;
}

bool leaf(node_t* node){
	return !node->left && !node->right;
}
#endif
