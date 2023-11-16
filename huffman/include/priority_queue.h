#ifndef PRIOQ_H
#define PRIOQ_H

#include <stdlib.h>
#include <stdbool.h>

#define parent(idx) ((idx / 2))
#define left(idx)   ((2 * idx))
#define right(idx)  ((2 * idx + 1))

#include "node.h"

typedef struct {
	node_t **arr;
	size_t idx, size;
} prioq_t;

prioq_t* prioq_create(const size_t size) {
	prioq_t* pq = (prioq_t*)malloc(sizeof(prioq_t));
	pq->arr = (node_t**)malloc(sizeof(node_t*) * (size + 1));
	for(size_t i = 0; i < size + 1; i++)
		pq->arr[i] = NULL;
	pq->idx = 1;
	pq->size = size + 1;
	return pq;
}

void enqueue(prioq_t* pq, node_t* node){
	if(pq->idx == pq->size)
		return;
	pq->arr[pq->idx++] = node;
	int idx = pq->idx - 1;
	while(idx > 0) {
		if(pq->arr[parent(idx)] &&
				pq->arr[parent(idx)]->freq > pq->arr[idx]->freq){
		node_t* tmp = pq->arr[parent(idx)];
		pq->arr[parent(idx)] = pq->arr[idx];
		pq->arr[idx] = tmp;
		idx = parent(idx);
		}
		else break;
	}
}

node_t* dequeue(prioq_t* pq) {
	if(pq->idx == 1)
		return NULL;
	node_t* root = pq->arr[1];
	pq->arr[1] = pq->arr[--pq->idx];
	size_t idx = 1;
	while(true){
		size_t smaller = idx;
		if(left(idx) <= pq->idx && 
				pq->arr[left(idx)] &&
				pq->arr[left(idx)]->freq < pq->arr[smaller]->freq)
			smaller = left(idx);
		if(right(idx) <= pq->idx &&
				pq->arr[right(idx)] &&
				pq->arr[right(idx)]->freq < pq->arr[smaller]->freq)
			smaller = right(idx);
		if(smaller == idx) break;
		node_t* tmp = pq->arr[smaller];
		pq->arr[smaller] = pq->arr[idx];
		pq->arr[idx] = tmp;
		idx = smaller;
	}
	return root;
}

void prioq_destroy(prioq_t* pq){
	if(pq)
		if(pq->arr){
			for(size_t idx = 0; idx < pq->idx; idx++){
				if(pq->arr[idx])
					free(pq->arr[idx]);
				pq->arr[idx] = NULL;
			}
			free(pq->arr);
		}
	free(pq);
}

#endif 
