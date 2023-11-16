#ifndef heap_H
#define heap_H

/*
 *
 * Modifikovana implementacija heap-a koja je prethodno
 * koriscena za huffmanovo kodiranje
 *
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define parent(idx) ((idx / 2))
#define left(idx)   ((2 * idx))
#define right(idx)  ((2 * idx + 1))

typedef struct {
	int16_t *arr;
	size_t idx, size;
} heap_t;

heap_t* heap_create(const size_t size) {
	heap_t* heap = (heap_t*)malloc(sizeof(heap_t));
	heap->arr = (int16_t*)malloc(sizeof(int16_t*) * (size + 1));
	heap->idx = 1;
	heap->size = size + 1;
	return heap;
}

void enqueue(heap_t* heap, int16_t number){
	if(heap->idx == heap->size)
		return;
	heap->arr[heap->idx++] = number;
	int idx = heap->idx - 1;
	while(idx > 0) {
		if(heap->arr[parent(idx)] > heap->arr[idx]){
			int16_t tmp = heap->arr[parent(idx)];
			heap->arr[parent(idx)] = heap->arr[idx];
			heap->arr[idx] = tmp;
			idx = parent(idx);
		}
		else break;
	}
}

int16_t dequeue(heap_t* heap) {
	if(heap->idx == 1)
		return -1;
	int16_t root = heap->arr[1];
	heap->arr[1] = heap->arr[--heap->idx];
	size_t idx = 1;
	while(true){
		size_t smaller = idx;
		if(left(idx) <= heap->idx &&
				heap->arr[left(idx)] < heap->arr[smaller])
			smaller = left(idx);
		if(right(idx) <= heap->idx &&
				heap->arr[right(idx)] < heap->arr[smaller])
			smaller = right(idx);
		if(smaller == idx) break;
		int16_t tmp = heap->arr[smaller];
		heap->arr[smaller] = heap->arr[idx];
		heap->arr[idx] = tmp;
		idx = smaller;
	}
	return root;
}

bool heap_empty(heap_t* heap) {
	return heap->idx == 1;
}

void heap_destroy(heap_t* heap){
	if(heap)
		if(heap->arr)
			free(heap->arr);
	free(heap);
}

#endif 
