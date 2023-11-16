#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "../include/fib_heap.h"

#define MAX_VALUE 100

int main() {
	srand(time(0));
	fib_heap_t* heap = create_fib_heap();
	char filename[50] = {0};
	char dotCommand[128] = {0};
	size_t cvorovi = 0;
	printf("Broj cvorova: ");
	scanf("%lu", &cvorovi);
	for(size_t i = 0; i < cvorovi; i++)
		fib_heap_insert(heap, rand() % MAX_VALUE);
	for(size_t i = 0; i < cvorovi; i++) {
		sprintf(filename, "heap%02lu.dot", i);
		FILE* fptr = fopen(filename, "w");
		dot_print(heap, fptr);
		fclose(fptr);
		sprintf(dotCommand, "dot -Tpng -Gsize=15,9\\! -Gdpi=100 heap%02lu.dot > heap%02lu.png", i, i);
		system(dotCommand);
		fib_heap_extract_min(heap);
	}
	fib_heap_destroy(heap);
	return 0;
}
