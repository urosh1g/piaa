#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <assert.h>

#define NUM_CASES 4
#define MB40 40000000

#include "../include/ssearch.h"

char* hex_cases[NUM_CASES] = {
	"../tests/hex100.txt",
	"../tests/hex1000.txt",
	"../tests/hex10000.txt",
	"../tests/hex100000.txt"
};

char* ascii_cases[NUM_CASES] = {
	"../tests/ascii100.txt",
	"../tests/ascii1000.txt",
	"../tests/ascii10000.txt",
	"../tests/ascii100000.txt"
};

size_t word_len[] = { 5, 10, 20, 50 };
size_t word_len_sz = sizeof(word_len) / sizeof(word_len[0]);

int main(int argc, char** argv) {
	srand(time(0));
	FILE* fd = NULL, *log = stdout, *matches = stdout;
	char* buffer = (char*)malloc(MB40);
	char* word;
	double times[2] = {.0f};
	size_t times_better[2] = {0};

	if(argc >= 2) {
		if(!(log = fopen(argv[1], "w"))){
			perror(strerror(errno));
			exit(1);
		}
	}
	if(argc == 3) {
		if(!(matches = fopen(argv[2], "w"))){
			perror(strerror(errno));
			exit(1);
		}
	}
	for(size_t i = 0; i < NUM_CASES; i++) {
		size_t b_len = 0;
		fd = fopen(hex_cases[i], "r");
		assert(fd != NULL);
		char ch;
		while(1) {
			fscanf(fd, "%c", &ch);
			if(feof(fd)) break;
			buffer[b_len++] = ch;
		}
		fclose(fd);
		for(size_t j = 0; j < word_len_sz; j++) {
			size_t w_len = word_len[j];
			size_t low = 0, high = b_len - w_len - 1;
			size_t pos = rand() % (high - low + 1) + low;
			word = (char*)malloc((w_len + 1) * sizeof(char));
			strncpy(word, buffer + pos, w_len);

			clock_t t_start = clock();
			kmp(matches, buffer, word);
			clock_t t_end = clock();
			times[0] = 1000 * (double)(t_end - t_start) / CLOCKS_PER_SEC;

			fprintf(log, "KMP FINISHED IN %.2fms (testcase %s | word_len %lu)\n", times[0], hex_cases[i], w_len);

			size_t iter = 0;
			t_start = clock();
			while(iter < (b_len - w_len)) {
				if(iter + w_len >= b_len) break;
				char* tmp_word = (char*)malloc((w_len + 1) * sizeof(char));
				strncpy(tmp_word, buffer + iter, w_len);
				if(levenstein(tmp_word, word, 0) <= 3)
					fprintf(matches, "LEVENSTEIN found match at %lu (word=%s, substr=%s)\n", iter, word, tmp_word);
				
				iter += w_len;
				free(tmp_word);
			}
			t_end = clock();
			times[1] = 1000 * (double)(t_end - t_start) / CLOCKS_PER_SEC;
			fprintf(log, "LEVENSTEIN FINISHED IN %.2fms (testcase %s | word_len %lu)\n", times[1], hex_cases[i], w_len);
			free(word);
			if(times[0] < times[1]){
				times_better[0]++;
				printf("KMP IS BETTER WITH %.2fms (LEV %.2fms)\n", times[0], times[1]);
			}
			else {
				times_better[1]++;
				printf("LEV IS BETTER WITH %.2fms (KMP %.2fms)\n", times[1], times[0]);
			}
		}
	}

	for(size_t i = 0; i < NUM_CASES; i++) {
		size_t b_len = 0;
		fd = fopen(ascii_cases[i], "r");
		assert(fd != NULL);
		char ch;
		while(1) {
			fscanf(fd, "%c", &ch);
			if(feof(fd)) break;
			buffer[b_len++] = ch;
		}
		fclose(fd);
		for(size_t j = 0; j < word_len_sz; j++) {
			size_t w_len = word_len[j];
			size_t low = 0, high = b_len - w_len - 1;
			size_t pos = rand() % (high - low + 1) + low;
			word = (char*)malloc((w_len + 1) * sizeof(char));
			strncpy(word, buffer + pos, w_len);

			clock_t t_start = clock();
			kmp(matches, buffer, word);
			clock_t t_end = clock();
			times[0] = 1000 * (double)(t_end - t_start) / CLOCKS_PER_SEC;

			fprintf(log, "KMP FINISHED IN %.2fms (testcase %s | word_len %lu)\n", times[0], hex_cases[i], w_len);

			size_t iter = 0;
			t_start = clock();
			while(iter < (b_len - w_len)) {
				if(iter + w_len >= b_len) break;
				char* tmp_word = (char*)malloc((w_len + 1) * sizeof(char));
				strncpy(tmp_word, buffer + iter, w_len);
				if(levenstein(tmp_word, word, 0) <= 3)
					fprintf(matches, "LEVENSTEIN found match at %lu (word=%s, substr=%s)\n", iter, word, tmp_word);
				
				iter += w_len;
				free(tmp_word);
			}
			t_end = clock();
			times[1] = 1000 * (double)(t_end - t_start) / CLOCKS_PER_SEC;
			fprintf(log, "LEVENSTEIN FINISHED IN %.2fms (testcase %s | word_len %lu)\n", times[1], hex_cases[i], w_len);
			free(word);
			if(times[0] < times[1]){
				times_better[0]++;
				printf("KMP IS BETTER WITH %.2fms (LEV %.2fms)\n", times[0], times[1]);
			}
			else {
				times_better[1]++;
				printf("LEV IS BETTER WITH %.2fms (KMP %.2fms)\n", times[1], times[0]);
			}
		}
	}

	if(times_better[0] > times_better[1])
		printf("KMP WINS (%lu times better time)\n", times_better[0]);
	if(times_better[0] < times_better[1])
		printf("LEVENSTEIN WINS (%lu times better time)\n", times_better[1]);
	free(buffer);
	if(log != stdout) fclose(log);
	if(matches != stdout) fclose(matches);
	return 0;
}
