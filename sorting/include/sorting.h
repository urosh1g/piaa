#ifndef SORTING_H
#define SORTING_H

#include <stdio.h>

//za variable-size intove, printf i scanf makroe
#include <inttypes.h>

int16_t* heap_sort(const int16_t* array, size_t nelem, double* time, size_t* mem_usage);
int16_t* insertion_sort(const int16_t* array, size_t nelem, double* time, size_t* mem_usage);
int16_t* counting_sort(const int16_t* array, size_t nelem, double* time, size_t* mem_usage);

#endif
