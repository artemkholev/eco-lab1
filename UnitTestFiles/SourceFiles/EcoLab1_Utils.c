#include "EcoLab1_Utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "IdEcoMemoryManager1.h"

/* Компараторы */
int compInts(const void *a_ptr, const void *b_ptr) {
    int a = *(const int *)a_ptr;
    int b = *(const int *)b_ptr;
    return (a > b) - (a < b);
}

int compFloats(const void *a_ptr, const void *b_ptr) {
    float a = *(const float *)a_ptr;
    float b = *(const float *)b_ptr;
    return (a > b) - (a < b);
}

int compDoubles(const void *a_ptr, const void *b_ptr) {
    double a = *(const double *)a_ptr;
    double b = *(const double *)b_ptr;
    return (a > b) - (a < b);
}

int compStrings(const void *a_ptr, const void *b_ptr) {
    const char *a = *(const char **)a_ptr;
    const char *b = *(const char **)b_ptr;
    return strcmp(a, b);
}

/* Печать массивов */
void printIntArray(void *array, size_t size) {
    int *arr = (int *) array;
    for (size_t i = 0; i < size; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void printFloatArray(void *array, size_t size) {
    float *arr = (float *) array;
    for (size_t i = 0; i < size; ++i) {
        printf("%f ", arr[i]);
    }
    printf("\n");
}

void printDoubleArray(void *array, size_t size) {
    double *arr = (double *) array;
    for (size_t i = 0; i < size; ++i) {
        printf("%lf ", arr[i]);
    }
    printf("\n");
}

void printStringArray(void *array, size_t size) {
    char **arr = (char **) array;
    for (size_t i = 0; i < size; ++i) {
        printf("'%s' ", arr[i]);
    }
    printf("\n");
}

/* Генерация случайных элементов */
void generateRandomInt(void *ptr) {
    *(int*)ptr = rand() % 20003 - 10000;
}

void generateRandomFloat(void *ptr) {
    *(float*)ptr = ((float)(rand() % 20003 - 10000)) / ((float)(rand() % 1000 + 1));
}

void generateRandomDouble(void *ptr) {
    *(double*)ptr = ((double)(rand() % 20003 - 10000)) / ((double)(rand() % 1000 + 1));
}

void generateRandomString(void *ptr, size_t maxLength, IEcoMemoryAllocator1 *pIMem) {
    char **strPtr = (char**)ptr;
    size_t len = rand() % (maxLength - 4 + 1) + 4;
    *strPtr = (char*)pIMem->pVTbl->Alloc(pIMem, (len + 1) * sizeof(char));
    for (size_t i = 0; i < len; ++i) (*strPtr)[i] = 'a' + rand() % 26;
    (*strPtr)[len] = 0;
}
