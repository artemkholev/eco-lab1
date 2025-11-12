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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "EcoLab1_Utils.h"
#include "EcoLab1_Test.h"
#include "EcoLab1_Generators.h"

#include "IdEcoCalculatorA.h"
#include "IdEcoCalculatorB.h"
#include "IdEcoCalculatorD.h"
#include "IdEcoCalculatorE.h"
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"

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
    int arraySizes[] = {80000, 90000, 100000, 200000, 300000, 400000};
     /* Указатель на интерфейс X */
    IEcoCalculatorX* pIX = 0;
    /* Указатель на интерфейс Y */
    IEcoCalculatorY* pIY = 0;
    /* Результат математических операций */
    int32_t comp_result = 0;

    /* Проверка и создание системного интрефейса */
    if (pISys == 0) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void**)&pISys);
        if (result != 0 && pISys == 0) {
            /* Освобождение системного интерфейса в случае ошибки */
            goto Release;
        }
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void**)&pIBus);
    if (result != 0 || pIBus == 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#ifdef ECO_LIB
    /* Регистрация статического компонента для работы со списком */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoLab1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_1F5DF16EE1BF43B999A434ED38FE8F3A);
    if (result != 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorB, (IEcoUnknown*)GetIEcoComponentFactoryPtr_AE202E543CE54550899603BD70C62565);
    if (result != 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorA, (IEcoUnknown*)GetIEcoComponentFactoryPtr_4828F6552E4540E78121EBD220DC360E);
    if (result != 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorD, (IEcoUnknown*)GetIEcoComponentFactoryPtr_3A8E44677E82475CB4A3719ED8397E61);
    if (result != 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorE, (IEcoUnknown*)GetIEcoComponentFactoryPtr_872FEF1DE3314B87AD44D1E7C232C2F0);
    if (result != 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#endif
    /* Получение интерфейса управления памятью */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**)&pIMem);

    /* Проверка */
    if (result != 0 || pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        goto Release;
    }

    /* Получение тестируемого интерфейса */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0, &IID_IEcoLab1, (void**)&pIEcoLab1);
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

    fclose(resultFile);

    printf("\n--- Checking Calculator Operations ---\n\n");

    // Get IEcoCalculatorX interface
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void**)&pIX);
    if (result != 0 || pIX == 0) {
        printf("Failed to get IEcoCalculatorX interface\n");
        goto Release;
    }

    // Get IEcoCalculatorY interface
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorY, (void**)&pIY);
    if (result != 0 || pIY == 0) {
        printf("Failed to get IEcoCalculatorY interface\n");
        goto Release;
    }

    // Test 1: Addition through CalculatorX interface
    comp_result = pIX->pVTbl->Addition(pIX, 7, 18);
    printf("[Addition Test] 7 + 18 => Result: %d ", comp_result);
    if (comp_result == 25) {
        printf("(OK)\n");
    } else if (comp_result == INT32_MIN) {
        printf("(ERROR: Invalid pointer)\n");
    }
    else {
        printf("(FAIL: Expected 25)\n");
    }

    // Test 2: Subtraction through CalculatorX interface
    comp_result = pIX->pVTbl->Subtraction(pIX, 42, 17);
    printf("[Subtraction Test] 42 - 17 => Result: %d ", comp_result);
    if (comp_result == 25) {
        printf("(OK)\n");
    }
    else if (comp_result == INT32_MIN) {
        printf("(ERROR: Invalid pointer)\n");
    }
    else {
        printf("(FAIL: Expected 25)\n");
    }

    // Test 3: Multiplication through CalculatorY interface
    comp_result = pIY->pVTbl->Multiplication(pIY, 6, 7);
    printf("[Multiplication Test] 6 * 7 => Result: %d ", comp_result);
    if (comp_result == 42) {
        printf("(OK)\n");
    }
    else if (comp_result == INT32_MIN) {
        printf("(ERROR: Invalid pointer)\n");
    }
    else {
        printf("(FAIL: Expected 42)\n");
    }

    // Test 4: Division through CalculatorY interface
    comp_result = pIY->pVTbl->Division(pIY, 56, 8);
    printf("[Division Test] 56 / 8 => Result: %d ", comp_result);
    if (comp_result == 7) {
        printf("(OK)\n");
    }
    else if (comp_result == INT32_MIN) {
        printf("(ERROR: Invalid pointer or zero division)\n");
    }
    else {
        printf("(FAIL: Expected 7)\n");
    }

    pIX->pVTbl->Release(pIX);
    pIY->pVTbl->Release(pIY);

    printf("\n--- QueryInterface Verification ---\n\n");

    // Query IEcoCalculatorX from IEcoLab1
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void**)&pIX);
    if (result == 0) {
        pIX->pVTbl->Release(pIX);
        printf("QueryInterface: IEcoLab1 => IEcoCalculatorX ... OK\n");
    }

    // Query IEcoCalculatorY from IEcoLab1
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorY, (void**)&pIY);
    if (result == 0) {
        pIY->pVTbl->Release(pIY);
        printf("QueryInterface: IEcoLab1 => IEcoCalculatorY ... OK\n");
    }

    // Query IEcoLab1 from itself
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoLab1, (void**)&pIEcoLab1);
    if (result == 0) {
        pIEcoLab1->pVTbl->Release(pIEcoLab1);
        printf("QueryInterface: IEcoLab1 => IEcoLab1 ... OK\n");
    }

    // Query IEcoCalculatorY from IEcoCalculatorX
    result = pIX->pVTbl->QueryInterface(pIX, &IID_IEcoCalculatorY, (void**)&pIY);
    if (result == 0) {
        pIY->pVTbl->Release(pIY);
        printf("QueryInterface: IEcoCalculatorX => IEcoCalculatorY ... OK\n");
    }

    // Query IEcoLab1 from IEcoCalculatorX
    result = pIX->pVTbl->QueryInterface(pIX, &IID_IEcoLab1, (void**)&pIEcoLab1);
    if (result == 0) {
        pIEcoLab1->pVTbl->Release(pIEcoLab1);
        printf("QueryInterface: IEcoCalculatorX => IEcoLab1 ... OK\n");
    }

    // Query IEcoCalculatorX from itself
    result = pIX->pVTbl->QueryInterface(pIX, &IID_IEcoCalculatorX, (void**)&pIX);
    if (result == 0) {
        pIX->pVTbl->Release(pIX);
        printf("QueryInterface: IEcoCalculatorX => IEcoCalculatorX ... OK\n");
    }

    // Query IEcoCalculatorX from IEcoCalculatorY
    result = pIY->pVTbl->QueryInterface(pIY, &IID_IEcoCalculatorX, (void**)&pIX);
    if (result == 0) {
        pIX->pVTbl->Release(pIX);
        printf("QueryInterface: IEcoCalculatorY => IEcoCalculatorX ... OK\n");
    }

    // Query IEcoCalculatorY from itself
    result = pIY->pVTbl->QueryInterface(pIY, &IID_IEcoCalculatorY, (void**)&pIY);
    if (result == 0) {
        pIY->pVTbl->Release(pIY);
        printf("QueryInterface: IEcoCalculatorY => IEcoCalculatorY ... OK\n");
    }

    // Query IEcoLab1 from IEcoCalculatorY
    result = pIY->pVTbl->QueryInterface(pIY, &IID_IEcoLab1, (void**)&pIEcoLab1);
    if (result == 0) {
        pIEcoLab1->pVTbl->Release(pIEcoLab1);
        printf("QueryInterface: IEcoCalculatorY => IEcoLab1 ... OK\n");
    }

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