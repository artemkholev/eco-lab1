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
#include "IdEcoCalculatorC.h"
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
    IEcoCalculatorX* pIEcoCalculatorX = 0;
    IEcoCalculatorY* pIEcoCalculatorY = 0;

	/* Переменные для проверки операций сторонних компонентов */
	int16_t x = 6;
	int16_t y = 7;
	int16_t res;

    int arraySizes[] = {80000, 90000, 100000, 200000, 300000, 400000};

    /* Проверка и создание системного интрефейса */
    if (pISys == 0) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void **)&pISys);
        if (result != 0 || pISys == 0) {
            result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem1, (void **)&pISys);
        }
        if (result != 0 || pISys == 0) {
            goto Release;
        }
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        goto Release;
    }
    result = pIBus->pVTbl->Init(pIBus);
    if (result != 0) {
        goto Release;
    }
    /* Получение интерфейса управления памятью */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 || pIMem == 0) {
        goto Release;
    }

#ifdef ECO_LIB
    /* Регистрация статического компонента для работы со списком */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorA, (IEcoUnknown*)GetIEcoComponentFactoryPtr_4828F6552E4540E78121EBD220DC360E);
    if (result != 0 ) {
        goto Release;
    }
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorB, (IEcoUnknown*)GetIEcoComponentFactoryPtr_AE202E543CE54550899603BD70C62565);
    if (result != 0 ) {
        goto Release;
    }
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorD, (IEcoUnknown*)GetIEcoComponentFactoryPtr_3A8E44677E82475CB4A3719ED8397E61);
    if (result != 0 ) {
        goto Release;
    }
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorE, (IEcoUnknown*)GetIEcoComponentFactoryPtr_872FEF1DE3314B87AD44D1E7C232C2F0);
    if (result != 0 ) {
        goto Release;
    }
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoLab1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_1F5DF16EE1BF43B999A434ED38FE8F3A);
    if (result != 0 ) {
        goto Release;
    }
#endif

    /* Получение тестируемого интерфейса */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0, &IID_IEcoLab1, (void**) &pIEcoLab1);
    if (result != 0 || pIEcoLab1 == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        fprintf(stderr, "QueryComponent(CID_EcoLab1, IID_IEcoLab1) failed: 0x%04X\n", (uint16_t)result);
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


	printf("\nInterfaces:\n");

    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void **) &pIEcoCalculatorX);
    if (result != 0) {
		printf("IEcoCalculatorX from EcoLab1 - error");
        goto Release;
    }
	printf("IEcoCalculatorX from EcoLab1 - correct\n");
    pIEcoCalculatorX->pVTbl->Release(pIEcoCalculatorX);

    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorY, (void **) &pIEcoCalculatorY);
    if (result != 0) {
		printf("IEcoCalculatorY from EcoLab1 - error\n");
        goto Release;
    }
	printf("IEcoCalculatorY from EcoLab1 - correct\n");
    pIEcoCalculatorY->pVTbl->Release(pIEcoCalculatorY);

	result = pIEcoCalculatorY->pVTbl->QueryInterface(pIEcoCalculatorY, &IID_IEcoCalculatorX, (void **) &pIEcoCalculatorX);
	if (result != 0) {
		printf("IEcoCalculatorX from IEcoCalculatorY - error\n");
        goto Release;
    }
	printf("IEcoCalculatorX from IEcoCalculatorY - correct\n");
    pIEcoCalculatorX->pVTbl->Release(pIEcoCalculatorX);

	/* запрос интерфейса IEcoCalculatorY через IEcoCalculatorX */
	result = pIEcoCalculatorX->pVTbl->QueryInterface(pIEcoCalculatorX, &IID_IEcoCalculatorY, (void **) &pIEcoCalculatorY);
	if (result != 0) {
		printf("IEcoCalculatorY from IEcoCalculatorX - error\n");
        goto Release;
    }
	printf("IEcoCalculatorY from IEcoCalculatorX - correct\n");
    pIEcoCalculatorY->pVTbl->Release(pIEcoCalculatorY);


	printf("\nOperations:\n");

	/* запрос интерфейса IEcoCalculatorX через IEcoLab1 */
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void **) &pIEcoCalculatorX);
    if (result != 0 || pIEcoCalculatorX == 0) {
        goto Release;
    }

	printf("\nAddition test\n");
	res = pIEcoCalculatorX->pVTbl->Addition(pIEcoCalculatorX, x, y);
	if (res == x + y) {
		printf("%d + %d == %4d\n", x, y, res);
	} else {
		printf("%d + %d == %4d - error\n", x, y, res);
	}

	printf("\nSubtraction\n");
	res = pIEcoCalculatorX->pVTbl->Subtraction(pIEcoCalculatorX, x, y);
	if (res == x - y) {
		printf("%d - %d == %4d\n", x, y, res);
	} else {
		printf("%d - %d == %4d - error\n", x, y, res);
	}
    pIEcoCalculatorX->pVTbl->Release(pIEcoCalculatorX);

	 /* запрос интерфейса IEcoCalculatorY через IEcoLab1 */
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorY, (void **) &pIEcoCalculatorY);
    if (result != 0 || pIEcoCalculatorY == 0) {
        goto Release;
    }

	printf("\nMultiplication\n");
	res = pIEcoCalculatorY->pVTbl->Multiplication(pIEcoCalculatorY, x, y);
	if (res == x * y) {
		printf("%d * %d == %4d\n", x, y, res);
	} else {
		printf("%d * %d == %4d - error\n", x, y, res);
	}

	printf("\nDivision\n");
	res = pIEcoCalculatorY->pVTbl->Division(pIEcoCalculatorY, x, y);
	if (res == x / y) {
		printf("%d / %d == %4d\n", x, y, res);
	} else {
		printf("%d / %d == %4d - error\n", x, y, res);
	}
	printf("\n");
    pIEcoCalculatorY->pVTbl->Release(pIEcoCalculatorY);

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
