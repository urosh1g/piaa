#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_ELEM 10000000
#define MAX_VALUE 10000

#include "../include/sorting.h"

typedef int16_t* (*sort_method)(const int16_t*, size_t, double*, size_t*);

enum SORT_METHOD {
	HEAP = 0,
	INSERTION,
	COUNTING,
	SORTERS_COUNT
};

sort_method sorters[] = {
	[HEAP] = heap_sort,
	[INSERTION] = insertion_sort,
	[COUNTING] = counting_sort
};

char* sorter_name(size_t idx);
void generate_numbers(int16_t** arr, size_t* count);
void print_array(int16_t* arr, size_t size);
void fprint_array(FILE* fd, int16_t* arr, size_t size);

int main(int argc, char** argv) {
	FILE* sink = NULL;
	if(argc == 2) {
		sink = fopen(argv[1], "w");
		if(!sink) {
			fprintf(stderr, "ERROR: cannot open output file\n");
			exit(1);
		}
	}
	else
		sink = stdout;

	int16_t* array = NULL, *sorted = NULL;
	size_t   array_size = 0, mem_usage[SORTERS_COUNT] = {0};
	double   times[SORTERS_COUNT] = {.0f};

	for(size_t i = 10; i <= MAX_ELEM; i *= 10){
		array_size = i;
		generate_numbers(&array, &array_size);
		for(size_t i = 0; i < SORTERS_COUNT; i++){
			sorted = sorters[i](array, array_size, &times[i], &mem_usage[i]);
			free(sorted);
		}
		fprintf(sink, "Times for sorting %lu elements:\n", array_size);
		fprintf(sink, "heap sort -> %.2fs, [%luKB]\n", times[HEAP], mem_usage[HEAP] / 1000);
		fprintf(sink, "insertion sort -> %.2fs, [%luKB]\n", times[INSERTION], mem_usage[INSERTION] / 1000);
		fprintf(sink, "counting sort -> %.2fs, [%luKB]\n\n", times[COUNTING], mem_usage[COUNTING] / 1000);
		bzero(mem_usage, sizeof(mem_usage) / sizeof(mem_usage[0]));
	}
	return 0;
}

char* sorter_name(size_t idx) {
	switch(idx){
		case HEAP:
			return "HEAP";
		case INSERTION:
			return "INSERTION";
		case COUNTING:
			return "COUNTING";
		default:
			return "ERROR";
	}
}

void generate_numbers(int16_t** arr, size_t* count) {
	if(*arr)
		free(*arr);
	*arr = (int16_t*)malloc(*count * sizeof(int16_t));
	for(size_t i = 0; i < *count; i++)
		(*arr)[i] = rand() % MAX_VALUE;
}

void print_array(int16_t* arr, size_t size) {
	for(size_t i = 0; i < size; i++)
		printf("%"PRId16" ", arr[i]);
	printf("\n");
}

void fprint_array(FILE* fd, int16_t* arr, size_t size) {
	for(size_t i = 0; i < size; i++)
		fprintf(fd, "%"PRId16"\n", arr[i]);
	printf("\n");
}
