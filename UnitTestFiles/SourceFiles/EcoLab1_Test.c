#include "EcoLab1_Test.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

void testSorting(IEcoLab1 *pIEcoLab1,
                 IEcoMemoryAllocator1 *pIMem,
                 FILE *file,
                 int seriousArraySize,
                 size_t elementSize,
                 const char *typeName,
                 GeneratorFunc generator,
                 PrintFunc printer,
                 ComparatorFunc comparator)
{
    const int simpleArraySize = 7;
    clock_t before, after;
    double shellDuration, qsortDuration;

    // проверка
    void *arr = pIMem->pVTbl->Alloc(pIMem, simpleArraySize * elementSize);
    generator(arr, simpleArraySize);

    printf("%s array before sorting:\n", typeName);
    printer(arr, simpleArraySize);

    pIEcoLab1->pVTbl->ShellSort(pIEcoLab1, arr, simpleArraySize, elementSize, comparator);

    printf("%s array after sorting:\n", typeName);
    printer(arr, simpleArraySize);

    for (int i = 0; i < simpleArraySize - 1; ++i) {
        char *current = (char*)arr + i * elementSize;
        char *next = (char*)arr + (i+1) * elementSize;
        if (comparator(current, next) > 0) {
            printf("%s array not sorted properly!\n", typeName);
            pIMem->pVTbl->Free(pIMem, arr);
            return;
        }
    }
    printf("%s array sorted correctly.\n", typeName);
    pIMem->pVTbl->Free(pIMem, arr);

    // ShellSort
    arr = pIMem->pVTbl->Alloc(pIMem, seriousArraySize * elementSize);
    generator(arr, seriousArraySize);
    before = clock();
    pIEcoLab1->pVTbl->ShellSort(pIEcoLab1, arr, seriousArraySize, elementSize, comparator);
    after = clock();
    shellDuration = (double)(after - before) / CLOCKS_PER_SEC;
    pIMem->pVTbl->Free(pIMem, arr);

    // qsort
    arr = pIMem->pVTbl->Alloc(pIMem, seriousArraySize * elementSize);
    generator(arr, seriousArraySize);
    before = clock();
    qsort(arr, seriousArraySize, elementSize, comparator);
    after = clock();
    qsortDuration = (double)(after - before) / CLOCKS_PER_SEC;
    pIMem->pVTbl->Free(pIMem, arr);

    fprintf(file, "shellsort,%s,%d,%lf\n", typeName, seriousArraySize, shellDuration);
    fprintf(file, "qsort,%s,%d,%lf\n", typeName, seriousArraySize, qsortDuration);
}
