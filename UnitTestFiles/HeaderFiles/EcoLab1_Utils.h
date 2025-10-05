#ifndef ECO_LAB1_UTILS_H
#define ECO_LAB1_UTILS_H

#include <stddef.h>

/* Компараторы */
int compInts(const void *a_ptr, const void *b_ptr);
int compFloats(const void *a_ptr, const void *b_ptr);
int compDoubles(const void *a_ptr, const void *b_ptr);
int compStrings(const void *a_ptr, const void *b_ptr);

/* Печать массивов */
void printIntArray(void *array, size_t size);
void printFloatArray(void *array, size_t size);
void printDoubleArray(void *array, size_t size);
void printStringArray(void *array, size_t size);

#endif /* ECO_LAB1_UTILS_H */