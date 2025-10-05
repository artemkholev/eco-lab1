#ifndef ECO_LAB1_TESTS_H
#define ECO_LAB1_TESTS_H

#include "IEcoLab1.h"
#include "IdEcoMemoryManager1.h"
#include <stdio.h>
#include <stddef.h>

typedef void (*GeneratorFunc)(void *array, size_t size);
typedef void (*PrintFunc)(void *array, size_t size);
typedef int (*ComparatorFunc)(const void *, const void *);

void testSorting(IEcoLab1 *pIEcoLab1,
                 IEcoMemoryAllocator1 *pIMem,
                 FILE *file,
                 int seriousArraySize,
                 size_t elementSize,
                 const char *typeName,
                 GeneratorFunc generator,
                 PrintFunc printer,
                 ComparatorFunc comparator);

#endif /* ECO_LAB1_TESTS_H */
