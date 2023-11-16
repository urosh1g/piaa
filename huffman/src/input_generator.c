#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define NUM_CASES 6
#define LOW 0
#define HIGH 127

void generate_random(uint64_t num);

int main(){
	srand(time(NULL));
	uint64_t sizes[NUM_CASES] = { 100, 1000, 10000, 100000, 1000000, 10000000 };
	for(size_t i = 0; i < NUM_CASES; i++)
		generate_random(sizes[i]);
	return 0;
}

void generate_random(uint64_t num){
	char test_size[10];
	sprintf(test_size, "%lu", num);
	char filename[64] = "../tests/";
	strcat(filename, "test");
	strcat(filename, test_size);
	strcat(filename, ".txt");

	FILE* fd = fopen(filename, "w");
	if(!fd){
		fprintf(stderr, "Error opening file (%s) for writing!\n", filename);
		return;
	}
	printf("Generating testcase %s\n", filename);
	clock_t tstart = clock();
	while(num --> 0){
		char ch = (char)((rand() % (HIGH - LOW + 1)) + LOW);
		fprintf(fd, "%c", ch);
	}
	clock_t tend = clock();
	printf("Testcase finished (finished in : %.2fms)\n", 1000.0 * (tend - tstart) / CLOCKS_PER_SEC);
	fclose(fd);
}
