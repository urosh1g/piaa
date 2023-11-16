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

fib_node_t* fib_heap_insert(fib_heap_t* heap, node_t* value) {
	fib_node_t* fib_node = create_fib_node(value);

	assert(heap);
	if(heap->min)
		append(heap->min, fib_node);
	else
		heap->min = fib_node;

	if(heap->min->node->dist > fib_node->node->dist)
		heap->min = fib_node;

	heap->nodes++;
	return fib_node;
}

static void link(fib_node_t* parent, fib_node_t* child) {
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
	fib_node_t** A = (fib_node_t**)malloc(max_deg * sizeof(fib_node_t*));
	for(size_t i = 0; i < max_deg; i++)
		A[i] = NULL;

	fib_node_t* w = heap->min;
	do {
		fib_node_t* parent = w;
		size_t degree = parent->degree;
		while(A[degree]) {
			fib_node_t* child = A[degree];
			if(child == parent) break;
			if(child->node->dist < parent->node->dist) {
				fib_node_t* tmp = child;
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
			if(heap->min->node->dist > A[i]->node->dist)
				heap->min = A[i];
		}
	}
	assert(heap->min && "NISAM USPEO DA NADJEM MIN?");
	free(A);
}

node_t* fib_heap_extract_min(fib_heap_t* heap) {
	if(!heap->min) {
		if(heap->nodes == 0)
			return NULL;
		else
			assert(false && "NEMA PTR NA MIN, A POSTOJE CVOROVI?");
	}
	fib_node_t* min = heap->min;
	node_t* min_value = min->node;

	if(min->children) {
		fib_node_t* iter = min->children;
		do {
			fib_node_t* next = iter->right;
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

static void cut(fib_heap_t* heap, fib_node_t* child, fib_node_t* parent) {
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

static void cascading_cut(fib_heap_t* heap, fib_node_t* fib_node) {
	fib_node_t* parent = fib_node->parent;
	if(parent) {
		if(!fib_node->mark) {
			fib_node->mark = true;
		}
		else {
			cut(heap, fib_node, parent);
			cascading_cut(heap, parent);
		}
	}
}

static fib_node_t* search_for(fib_node_t* rootlist, node_t* node) {
	if(!rootlist) return NULL;
	fib_node_t* iter = rootlist;
	fib_node_t* ret = NULL;
	do {
		if(iter->node == node) return iter;
		if((ret = search_for(iter->children, node)))
			break;
		iter = iter->right;
	} while(iter != rootlist);
	return ret;
}

static fib_node_t* find_fib_node(fib_heap_t* heap, node_t* node) {
	return search_for(heap->min, node);
}

void fib_heap_decrease_key(fib_heap_t* heap, node_t* node, int32_t value) {
	if(value > node->dist) return;
	node->dist = value;
	fib_node_t* fib_node = find_fib_node(heap, node);
	if(!fib_node) return;
	fib_node_t* parent = fib_node->parent;
	if(parent && fib_node->node->dist < parent->node->dist) {
		cut(heap, fib_node, parent);
		cascading_cut(heap, parent);
	}
	if(fib_node->node->dist < heap->min->node->dist)
		heap->min = fib_node;
}

void fib_heap_delete_fib_node(fib_heap_t* heap, node_t* node) {
	fib_heap_decrease_key(heap, node, INT32_MIN);
	fib_heap_extract_min(heap);
}

void fib_heap_destroy(fib_heap_t* heap) {
	while(heap->min)
		fib_heap_extract_min(heap);
	free(heap);
}
