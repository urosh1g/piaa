#ifndef _S_SEARCH_H
#define _S_SEARCH_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void kmp(FILE* fd, const char* string, const char* word);
size_t levenstein(char* const string, char* const word, size_t depth);

#endif
