#include "EcoLab1_Generators.h"
#include <stdlib.h>

void generateInts(void *array, size_t size) {
    int *arr = (int*)array;
    for (size_t i = 0; i < size; i++) arr[i] = rand()%20003-10000;
}

void generateFloats(void *array, size_t size) {
    float *arr = (float*)array;
    for (size_t i = 0; i < size; i++) arr[i] = ((float)(rand()%20003-10000))/((float)(rand()%1000+1));
}

void generateDoubles(void *array, size_t size) {
    double *arr = (double*)array;
    for (size_t i = 0; i < size; i++) arr[i] = ((double)(rand()%20003-10000))/((double)(rand()%1000+1));
}

void generateStrings(void *array, size_t size) {
    char **arr = (char**)array;
    for (size_t i = 0; i < size; i++) {
        arr[i] = (char*)malloc(20);
        int len = rand()%14+4;
        for(int j=0;j<len;j++) arr[i][j]=(char)(rand()%('z'-'a'+1)+'a');
        arr[i][len]=0;
    }
}
