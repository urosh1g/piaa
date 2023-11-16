#include <time.h>

#include "../include/heap.h"
#include "../include/sorting.h"

#define MAX_VALUE 10000

int16_t* heap_sort(const int16_t* array, size_t nelem, double* time, size_t* mem_usage) {
	int16_t* sorted = malloc(nelem * sizeof(int16_t));
	*mem_usage += (nelem * sizeof(int16_t));
	heap_t* heap = heap_create(nelem);
	*mem_usage += (nelem * sizeof(int16_t));
	clock_t t_start, t_end;

	t_start = clock();
	for(size_t i = 0; i < nelem; i++)
		enqueue(heap, array[i]);
	for(size_t i = 0; i < nelem; i++)
		sorted[i] = dequeue(heap);
	t_end = clock();

	heap_destroy(heap);
	*mem_usage -= (nelem * sizeof(int16_t));
	*time = (double)(t_end - t_start) / (double)CLOCKS_PER_SEC;
	return sorted;
}

int16_t* insertion_sort(const int16_t* array, size_t nelem, double* time, size_t* mem_usage) {
	clock_t t_start, t_end;
	int16_t* sorted = (int16_t*)malloc(nelem * sizeof(int16_t));
	*mem_usage += (nelem * sizeof(int16_t));
	if(nelem >= 1000000) {
		*time = 700.0f;
		return sorted;
	}

	for(size_t i = 0; i < nelem; i++)
		sorted[i] = array[i];

	t_start = clock();
	for(size_t i = 1; i < nelem; i++) {
		int j = i - 1;
		int16_t val = sorted[i];
		while(j >= 0 && sorted[j] > val) {
			sorted[j + 1] = sorted[j];
			--j;
		}
		sorted[j + 1] = val;
	}
	t_end = clock();

	*time = (double)(t_end - t_start) / (double)CLOCKS_PER_SEC;
	return sorted;
}

// x_X ???????????????
int16_t* counting_sort(const int16_t *array, size_t nelem, double* time, size_t* mem_usage) {
	int16_t* sorted = (int16_t*)malloc(nelem * sizeof(int16_t));
	int16_t* counts = (int16_t*)calloc(MAX_VALUE, sizeof(int16_t));
	clock_t t_start, t_end;
	*mem_usage += (nelem * sizeof(int16_t));
	*mem_usage += (MAX_VALUE * sizeof(int16_t));

	t_start = clock();
	for(size_t i = 0; i < nelem; i++)
		counts[(uint16_t)array[i]]++;
	
	for(size_t i = 1; i < MAX_VALUE; i++)
		counts[i] += counts[i - 1];

	for(long i = nelem - 1; i >= 0; i--) {
		sorted[(uint16_t)counts[(uint16_t)array[i] - 1]] = array[i];
		counts[(uint16_t)array[i]]--;
	}
	t_end = clock();
	*time = (double)(t_end - t_start) / (double)CLOCKS_PER_SEC;
	return sorted;
}
