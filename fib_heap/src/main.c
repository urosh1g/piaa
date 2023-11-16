#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/fib_heap.h"

#define MAX_VALUE 100000

size_t tests[] = { 10, 100, 1000, 10000, 100000 };
size_t num_tests = sizeof(tests) / sizeof(tests[0]);

//vrati vreme u msec
static inline double cdiff(clock_t t_end, clock_t t_start) {
	return 1000 * (double)(t_end - t_start) / (double)CLOCKS_PER_SEC;
}

int main() {
	fib_heap_t* heap = create_fib_heap();
	node_t *first, *second, *third;
	double insert_time = 0, delete_time = 0, extract_time = 0;
	double total_time = 0;
	clock_t ins_start, ins_end;
	clock_t del_start, del_end;
	clock_t ext_start, ext_end;
	(void)first;

	for(size_t test = 0; test < num_tests; test++) {
		for(size_t size = 0; size < tests[size]; size++) {
			ins_start = clock();
			first = fib_heap_insert(heap, rand() % MAX_VALUE);
			second = fib_heap_insert(heap, rand() % MAX_VALUE);
			third = fib_heap_insert(heap, rand() % MAX_VALUE);
			ins_end = clock();
			insert_time += cdiff(ins_end, ins_start);

			del_start = clock();
			fib_heap_delete_node(heap, third);
			del_end = clock();
			delete_time += cdiff(del_end, del_start);

			ext_start = clock();
			fib_heap_extract_min(heap);
			ext_end = clock();
			extract_time += cdiff(ext_end, ext_start);

			ins_start = clock();
			first = fib_heap_insert(heap, rand() % MAX_VALUE);
			second = fib_heap_insert(heap, rand() % MAX_VALUE);
			ins_end = clock();
			insert_time += cdiff(ins_end, ins_start);

			del_start = clock();
			fib_heap_delete_node(heap, second);
			del_end = clock();
			delete_time += cdiff(del_end, del_start);

			ins_start = clock();
			first = fib_heap_insert(heap, rand() % MAX_VALUE);
			second = fib_heap_insert(heap, rand() % MAX_VALUE);
			ins_end = clock();
			insert_time += cdiff(ins_end, ins_start);

			ext_start = clock();
			fib_heap_extract_min(heap);
			ext_end = clock();
			extract_time += cdiff(ext_end, ext_start);

			ins_start = clock();
			first = fib_heap_insert(heap, rand() % MAX_VALUE);
			ins_end = clock();
			insert_time += cdiff(ins_end, ins_start);
		}

		insert_time /= (double)8;
		extract_time /= (double)2;
		delete_time /= (double)2;
		total_time += insert_time + extract_time + delete_time;
		printf("Cycle size %lu\nInsert avg:%.3f\nExtract min avg:%.3f\nDelete avg:%.3f\n", tests[test], (double)insert_time, (double)extract_time, (double)delete_time);
		insert_time = delete_time = extract_time = 0;
	}
	printf("Total time: %.3fms\n", total_time);
	return 0;
}
