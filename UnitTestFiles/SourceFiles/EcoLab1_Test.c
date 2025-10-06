#include "EcoLab1_Test.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

void testSorting(IEcoLab1 *pIEcoLab1,
                 IEcoMemoryAllocator1 *pIMem,
                 FILE *file,
                 int arraySize,
                 size_t elementSize,
                 const char *typeName,
                 GeneratorFunc generator,
                 PrintFunc printer,
                 ComparatorFunc comparator)
{
    const int testArraySize = 10;
    clock_t before, after;
    double shellDuration, qsortDuration;

    // проверка
    void *arr = pIMem->pVTbl->Alloc(pIMem, testArraySize * elementSize);
    generator(arr, testArraySize);

    printf("%s array:\n", typeName);
    printer(arr, testArraySize);

    pIEcoLab1->pVTbl->ShellSort(pIEcoLab1, arr, testArraySize, elementSize, comparator);

    printf("%s array after sorting:\n", typeName);
    printer(arr, testArraySize);

    for (int i = 0; i < testArraySize - 1; ++i) {
        char *current = (char*)arr + i * elementSize;
        char *next = (char*)arr + (i+1) * elementSize;
        if (comparator(current, next) > 0) {
            printf("%s array not sorted properly!\n", typeName);
            pIMem->pVTbl->Free(pIMem, arr);
            return;
        }
    }
    printf("%s sorted correctly.\n", typeName);
    pIMem->pVTbl->Free(pIMem, arr);

    // ShellSort
    arr = pIMem->pVTbl->Alloc(pIMem, arraySize * elementSize);
    generator(arr, arraySize);
    before = clock();
    pIEcoLab1->pVTbl->ShellSort(pIEcoLab1, arr, arraySize, elementSize, comparator);
    after = clock();
    shellDuration = (double)(after - before) / CLOCKS_PER_SEC;
    pIMem->pVTbl->Free(pIMem, arr);

    // qsort
    arr = pIMem->pVTbl->Alloc(pIMem, arraySize * elementSize);
    generator(arr, arraySize);
    before = clock();
    qsort(arr, arraySize, elementSize, comparator);
    after = clock();
    qsortDuration = (double)(after - before) / CLOCKS_PER_SEC;
    pIMem->pVTbl->Free(pIMem, arr);

    fprintf(file, "shellsort,%s,%d,%lf\n", typeName, arraySize, shellDuration);
    fprintf(file, "qsort,%s,%d,%lf\n", typeName, arraySize, qsortDuration);
}
