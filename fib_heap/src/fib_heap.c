#include <math.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../include/fib_heap.h"

fib_heap_t* create_fib_heap() {
	fib_heap_t* heap = (fib_heap_t*)malloc(sizeof(fib_heap_t));
	heap->min = NULL;
	heap->m = heap->t = 0;
	heap->nodes = 0;
	return heap;
}

node_t* fib_heap_insert(fib_heap_t* heap, int32_t value) {
	node_t* node = create_node(value);

	assert(heap);
	if(heap->min)
		append(heap->min, node);
	else
		heap->min = node;

	if(heap->min->value > node->value)
		heap->min = node;

	heap->nodes++;
	return node;
}

static void link(node_t* parent, node_t* child) {
	child->right->left = child->left;
	child->left->right = child->right;

	child->left = child->right = child;

	if(parent->children)
		append(parent->children, child);
	else
		parent->children = child;
	parent->degree++;
	child->parent = parent;
	child->mark = false;
}

static void consolidate(fib_heap_t* heap) {
	size_t max_deg = ceil(log2(heap->nodes)) + 1;
	node_t** A = (node_t**)malloc(max_deg * sizeof(node_t*));
	for(size_t i = 0; i < max_deg; i++)
		A[i] = NULL;

	node_t* w = heap->min;
	do {
		node_t* parent = w;
		size_t degree = parent->degree;
		while(A[degree]) {
			node_t* child = A[degree];
			if(child == parent) break;
			if(child->value < parent->value) {
				node_t* tmp = child;
				child = parent;
				parent = tmp;
			}
			if(child == heap->min)
				heap->min = heap->min->right;
			link(parent, child);
			assert(child->parent == parent && "LINK NIJE POSTAVIO PARENTA?");
			if(parent == parent->right)
				heap->min = parent;
			A[degree] = NULL;
			++degree;
		}
		A[degree] = parent;
		w = parent->right;
	} while(w != heap->min);

	heap->min = NULL;
	
	for(size_t i = 0; i < max_deg; i++) {
		if(A[i] != NULL) {
			A[i]->left = A[i]->right = A[i];
			if(heap->min == NULL) {
				heap->min = A[i];
			}
			else {
				append(heap->min, A[i]);
			}
			if(heap->min->value > A[i]->value)
				heap->min = A[i];
		}
	}
	assert(heap->min && "NISAM USPEO DA NADJEM MIN?");
	free(A);
}

int32_t fib_heap_extract_min(fib_heap_t* heap) {
	if(!heap->min) {
		if(heap->nodes == 0)
			return INT32_MAX;
		else
			assert(false && "NEMA PTR NA MIN, A POSTOJE CVOROVI?");
	}
	node_t* min = heap->min;
	int32_t min_value = min->value;

	if(min->children) {
		node_t* iter = min->children;
		do {
			node_t* next = iter->right;
			iter->left->right = iter->right;
			iter->right->left = iter->left;
			iter->left = iter->right = iter;
			assert(iter->parent && "JA SAM U LISTI DECE A NEMAM PTR NA RODITELJA?");
			iter->parent = NULL;
			append(heap->min, iter);
			if(iter == next) break;
			iter = next;
		} while(iter != min->children);
	}
	min->children = NULL;

	min->right->left = min->left;
	min->left->right = min->right;


	if(min == min->right)
		heap->min = NULL;
	else {
		heap->min = min->right;
		consolidate(heap);
	}
	heap->nodes--;
	free(min);
	return min_value;
}

static void cut(fib_heap_t* heap, node_t* child, node_t* parent) {
	child->parent = NULL;
	if(child == child->right)
		parent->children = NULL;
	else
		parent->children = child->right;

	child->left->right = child->right;
	child->right->left = child->left;
	parent->degree--;
	child->left = child->right = child;
	child->mark = false;
	append(heap->min, child);
}

static void cascading_cut(fib_heap_t* heap, node_t* node) {
	node_t* parent = node->parent;
	if(parent) {
		if(!node->mark) {
			node->mark = true;
		}
		else {
			cut(heap, node, parent);
			cascading_cut(heap, parent);
		}
	}
}

void fib_heap_decrease_key(fib_heap_t* heap, node_t* node, int32_t value) {
	if(value > node->value) return;
	node->value = value;
	node_t* parent = node->parent;
	if(parent && node->value < parent->value) {
		cut(heap, node, parent);
		cascading_cut(heap, parent);
	}
	if(node->value < heap->min->value)
		heap->min = node;
}

void fib_heap_delete_node(fib_heap_t* heap, node_t* node) {
	fib_heap_decrease_key(heap, node, INT32_MIN);
	fib_heap_extract_min(heap);
}

void print_node(node_t* node, FILE* sink) {
	if(!node->children) {
		return;
	}
	node_t* child = node->children;
	fprintf(sink, "    \"%"PRIxPTR"\" -> \"%"PRIxPTR"\" [color=darksalmon];\n", (uintptr_t)node, (uintptr_t)child);
	fprintf(sink, "    \"%"PRIxPTR"\" -> \"%"PRIxPTR"\" [color=darksalmon];\n", (uintptr_t)child, (uintptr_t)node);
	do {
		fprintf(sink, "    {\nrank=same;\n");
		fprintf(sink, "    \"%"PRIxPTR"\" [fillcolor=darksalmon label=\"%d\" style=filled];\n", (uintptr_t)child, child->value);
		fprintf(sink, "    \"%"PRIxPTR"\" -> \"%"PRIxPTR"\" [color=darkslategray];\n", (uintptr_t)child, (uintptr_t)child->right);
		fprintf(sink, "    \"%"PRIxPTR"\" -> \"%"PRIxPTR"\" [color=darkslategray];\n", (uintptr_t)child, (uintptr_t)child->left);
		fprintf(sink, "    }\n");
		print_node(child, sink);
		child = child->right;
	} while(child != node->children);
}

void dot_print(fib_heap_t* heap, FILE* sink) {
	assert(heap);
	fprintf(sink, "digraph FIB_HEAP {\n");
	fprintf(sink, "    ranksep=\"1.2 equally\"\n");
	fprintf(sink, "    newrank=\"true\"\n");
	node_t* iter = heap->min;
	if(!iter) { fprintf(sink, "}\n"); return; }
	do {
		fprintf(sink, "    {\nrank=same;\n");
		fprintf(sink, "    \"%"PRIxPTR"\" [fillcolor=firebrick2 label=\"%d\" style=filled];\n", (uintptr_t)iter, iter->value);
		fprintf(sink, "    \"%"PRIxPTR"\" -> \"%"PRIxPTR"\" [color=\"red\"];\n", (uintptr_t)iter, (uintptr_t)iter->right);
		fprintf(sink, "    \"%"PRIxPTR"\" -> \"%"PRIxPTR"\" [color=\"red\"];\n", (uintptr_t)iter, (uintptr_t)iter->left);
		fprintf(sink, "    }\n");
		print_node(iter, sink);
		iter = iter->right;
	} while(iter != heap->min);
	fprintf(sink, "}\n");
}

void fib_heap_destroy(fib_heap_t* heap) {
	while(heap->min)
		fib_heap_extract_min(heap);
	free(heap);
}
