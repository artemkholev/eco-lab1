/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   EcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный исходный файл является точкой входа
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */


/* Eco OS */
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoFileSystemManagement1.h"
#include "IdEcoLab1.h"

#include "IEcoCalculatorY.h"
#include "IEcoCalculatorX.h"
#include "IdEcoCalculatorA.h"
#include "IdEcoCalculatorB.h"
#include "IdEcoCalculatorD.h"
#include "IdEcoCalculatorE.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "EcoLab1_Utils.h"
#include "EcoLab1_Test.h"
#include "EcoLab1_Generators.h"

/*
 *
 * <сводка>
 *   Функция EcoMain
 * </сводка>
 *
 * <описание>
 *   Функция EcoMain - точка входа
 * </описание>
 *
 */
int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    /* Указатель на системный интерфейс */
    IEcoSystem1* pISys = 0;
    /* Указатель на интерфейс работы с системной интерфейсной шиной */
    IEcoInterfaceBus1* pIBus = 0;
    /* Указатель на интерфейс работы с памятью */
    IEcoMemoryAllocator1* pIMem = 0;
    char_t* name = 0;
    /* Указатель на тестируемый интерфейс */
    IEcoLab1* pIEcoLab1 = 0;
    IEcoCalculatorY* pIY = 0;
    IEcoCalculatorX* pIX = 0;
	int arraySizes[] = {80000, 90000, 100000, 200000, 300000, 400000};

    /* Проверка и создание системного интрефейса */
    if (pISys == 0) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void **)&pISys);
        if (result != 0 && pISys == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
            goto Release;
        }
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#ifdef ECO_LIB
    /* Регистрация статического компонента для работы со списком */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoLab1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_1F5DF16EE1BF43B999A434ED38FE8F3A);
    if (result != 0 ) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#endif
    /* Получение интерфейса управления памятью */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 || pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        goto Release;
    }

    /* Выделение блока памяти */
    name = (char_t *)pIMem->pVTbl->Alloc(pIMem, 10);

    /* Заполнение блока памяти */
    pIMem->pVTbl->Fill(pIMem, name, 'a', 9);


    /* Получение тестируемого интерфейса */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0, &IID_IEcoLab1, (void**) &pIEcoLab1);
    if (result != 0 || pIEcoLab1 == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        goto Release;
    }

    printf("Testing Shell sort vs stdlib qsort. Results: output.csv\n");

    FILE *resultFile = fopen("output.csv", "w");
    if (!resultFile) {
        perror("Cannot open output.csv");
        return -1;
    }
    fprintf(resultFile, "sort,type,size,time\n");

       for (size_t i = 0; i < sizeof(arraySizes)/sizeof(arraySizes[0]); i++) {
        testSorting(pIEcoLab1, pIMem, resultFile, arraySizes[i],
                    sizeof(int), "int", generateInts, printIntArray, compInts);

        testSorting(pIEcoLab1, pIMem, resultFile, arraySizes[i],
                    sizeof(float), "float", generateFloats, printFloatArray, compFloats);

        testSorting(pIEcoLab1, pIMem, resultFile, arraySizes[i],
                    sizeof(double), "double", generateDoubles, printDoubleArray, compDoubles);

        testSorting(pIEcoLab1, pIMem, resultFile, arraySizes[i],
                    sizeof(char*), "string", generateStrings, printStringArray, compStrings);
    }
	
	/* запрос интерфейса IEcoCalculatorY через IEcoLab1 */
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorY, (void **) &pIY);
    if (result != 0 || pIY == 0) {
        goto Release;
    }

	printf("IEcoCalculatorY test:\n");
	printf("Multiplication test 15 * 10 = %d\n", pIY->pVTbl->Multiplication(pIY, 15, 10));
    printf("Division test 15 / 10 = %d\n", pIY->pVTbl->Division(pIY, 15, 10));
    pIY->pVTbl->Release(pIY);

    /* запрос интерфейса IEcoCalculatorX через IEcoLab1 */
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void **) &pIX);
    if (result != 0 || pIX == 0) {
        goto Release;
    }

	printf("\nIEcoCalculatorX test:\n");
	printf("Addition test 15 + 10 = %d\n", pIX->pVTbl->Addition(pIX, 15, 10));
    printf("Subtraction test 15 - 10 = %d\n", pIX->pVTbl->Subtraction(pIX, 15, 10));
    pIX->pVTbl->Release(pIX);

	printf("\nInterface test:\n");

    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void **) &pIX);
    if (result == 0) {
        printf("Query IX from Lab1 - success\n");
        pIX->pVTbl->Release(pIX);
    }
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorY, (void **) &pIY);
    if (result == 0) {
        printf("Query IY from Lab1 - success\n");
        pIY->pVTbl->Release(pIY);
    }

	result = pIY->pVTbl->QueryInterface(pIY, &IID_IEcoCalculatorX, (void **) &pIX);
    if (result == 0) {
        printf("Query IX from IY - success\n");
        pIX->pVTbl->Release(pIX);
    }

	result = pIX->pVTbl->QueryInterface(pIX, &IID_IEcoCalculatorY, (void **) &pIY);
    if (result == 0) {
        printf("Query IY from IX - success\n");
        pIY->pVTbl->Release(pIY);
    }

    pIMem->pVTbl->Free(pIMem, name);

Release:

    /* Освобождение интерфейса для работы с интерфейсной шиной */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }

    /* Освобождение интерфейса работы с памятью */
    if (pIMem != 0) {
        pIMem->pVTbl->Release(pIMem);
    }

    /* Освобождение тестируемого интерфейса */
    if (pIEcoLab1 != 0) {
        pIEcoLab1->pVTbl->Release(pIEcoLab1);
    }


    /* Освобождение системного интерфейса */
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }

    return result;
}

