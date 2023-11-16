#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "../include/ssearch.h"

int min(int val1, int val2, int val3) {
	int minv = val1;
	if(val2 < val3){
		if(val2 < val1)
			minv = val2;
	}
	else
		if(val3 < val1)
			minv = val3;
	return minv;
}

void kmp(FILE* fd, const char* string, const char* word) {
	if(!fd)
		fd = stdout;
	int32_t s_idx = 0, w_idx = 0;
	int32_t w_len = strlen(word);
	int32_t s_len = strlen(string);

	int16_t* T = (int16_t*)malloc(sizeof(int16_t) * w_len);
	memset(T, -1, w_len * sizeof(int16_t));

	int16_t cnd = 0, i = 1;
	while(i < w_len) {
		if(word[i] == word[cnd]) {
			T[i] = T[cnd];
		}
		else {
			T[i] = cnd;
			while(cnd >= 0 && word[i] != word[cnd]) {
				cnd = T[cnd];
			}
		}
		i++;
		cnd++;
	}
	T[i] = cnd;

	while(s_idx < s_len) {
		if(word[w_idx] == string[s_idx]) {
			s_idx++;
			w_idx++;

			if(w_idx == w_len) {
				fprintf(fd, "KMP found match at %d (word=%s, substr=%s)\n", s_idx - w_idx, word, word);
				w_idx = T[w_idx];
			}
		}
		else {
			w_idx = T[w_idx];
			if(w_idx < 0) {
				w_idx++;
				s_idx++;
			}
		}
	}

	free(T);
}

size_t levenstein(char* const string, char* const word, size_t depth) {
	if(depth > 3) return 0;
	if(!string || !strcmp(string, "")) {
		assert(word != NULL);
		return strlen(word);
	}
	if(!word || !strcmp(word, "")) {
		assert(string != NULL);
		return strlen(string);
	}
	if(string[0] == word[0]) {
		return levenstein(string + 1, word + 1, depth);
	}
	return 1 + min(levenstein(string + 1, word, depth + 1),
					levenstein(string, word + 1, depth + 1),
					levenstein(string + 1, word + 1, depth + 1));
}
