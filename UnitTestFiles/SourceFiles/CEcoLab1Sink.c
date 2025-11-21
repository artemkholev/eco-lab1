/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoLab1Sink
 * </сводка>
 *
 * <описание>
 *   Данный исходный код описывает реализацию интерфейсов CEcoLab1Sink
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2016 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#include "CEcoLab1Sink.h"
#include "IEcoConnectionPointContainer.h"
#include <stdio.h>
#include <unistd.h>

/* ANSI Color Codes for Linux */
#define ANSI_COLOR_RED_BG     "\x1b[41m"
#define ANSI_COLOR_GREEN_BG   "\x1b[42m"
#define ANSI_COLOR_BLUE_BG    "\x1b[44m"
#define ANSI_COLOR_BRIGHT     "\x1b[1m"
#define ANSI_COLOR_RESET      "\x1b[0m"

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_QueryInterface(/* in */ struct IEcoLab1Events* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    if ( IsEqualUGUID(riid, &IID_IEcoLab1Events ) ) {
        *ppv = me;
        me->pVTbl->AddRef(me);
        return 0;
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoUnknown ) ) {
        *ppv = me;
        me->pVTbl->AddRef(me);
        return 0;
    }

    *ppv = 0;

    return -1;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1Sink_AddRef(/* in */ struct IEcoLab1Events* me) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

    pCMe->m_cRef++;
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1Sink_Release(/* in */ struct IEcoLab1Events* me) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoLab1Sink((IEcoLab1Events*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Вспомогательные функции для вывода массивов
 * </сводка>
 *
 */
void CEcoLab1Sink_printBeforeIntArray(const void *array, size_t n) {
    int16_t* arr = (int16_t*)array;
    size_t i;

    printf(ANSI_COLOR_RED_BG ANSI_COLOR_BRIGHT);
    for (i = 0; i < n; ++i) {
        printf("%d ", arr[i]);
    }
    printf(ANSI_COLOR_RESET);
    printf("\n");
}

void CEcoLab1Sink_printIntShellSortStep(const void *array, size_t count, size_t gap, size_t current_index) {
    int16_t* arr = (int16_t*)array;
    size_t i;

    printf(ANSI_COLOR_BLUE_BG);
    printf("Gap=%zu, Index=%zu: ", gap, current_index);
    for (i = 0; i < count; ++i) {
        if (i == current_index) {
            printf(ANSI_COLOR_RESET ANSI_COLOR_BRIGHT "[%d] " ANSI_COLOR_BLUE_BG, arr[i]);
        } else {
            printf("%d ", arr[i]);
        }
    }
    printf(ANSI_COLOR_RESET);
    printf("\n");
}

void CEcoLab1Sink_printAfterIntArray(const void *array, size_t n) {
    int16_t* arr = (int16_t*)array;
    size_t i;

    printf(ANSI_COLOR_GREEN_BG ANSI_COLOR_BRIGHT);
    for (i = 0; i < n; ++i) {
        printf("%d ", arr[i]);
    }
    printf(ANSI_COLOR_RESET);
    printf("\n");
}

/*
 *
 * <сводка>
 *   Функция OnBeforeShellSort
 * </сводка>
 *
 * <описание>
 *   Функция обратного вызова перед началом сортировки
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_OnBeforeShellSort(/* in */ struct IEcoLab1Events* me, /* in */ const void *array, size_t count) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

    printf("\n=== Event: OnBeforeShellSort ===\n");
    printf("Array before sorting (%zu elements): ", count);
    CEcoLab1Sink_printBeforeIntArray(array, count);

    usleep(500000); /* 500ms delay */

    return 0;
}

/*
 *
 * <сводка>
 *   Функция ShellSortStep
 * </сводка>
 *
 * <описание>
 *   Функция обратного вызова для каждого шага сортировки Shell sort
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_ShellSortStep(struct IEcoLab1Events* me, const void *array, size_t count, size_t gap, size_t current_index) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

    printf("=== Event: ShellSortStep ===\n");
    CEcoLab1Sink_printIntShellSortStep(array, count, gap, current_index);

    usleep(100000); /* 100ms delay */

    return 0;
}

/*
 *
 * <сводка>
 *   Функция OnAfterShellSort
 * </сводка>
 *
 * <описание>
 *   Функция обратного вызова после завершения сортировки
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_OnAfterShellSort(struct IEcoLab1Events* me, const void *array, size_t count) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)me;

    if (me == 0 ) {
        return -1;
    }

    printf("=== Event: OnAfterShellSort ===\n");
    printf("Array after sorting (%zu elements): ", count);
    CEcoLab1Sink_printAfterIntArray(array, count);
    printf("\n");

    usleep(500000); /* 500ms delay */

    return 0;
}

/*
 *
 * <сводка>
 *   Функция Advise
 * </сводка>
 *
 * <описание>
 *   Функция подключения приемника к источнику событий
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_Advise(/* in */ struct CEcoLab1Sink* me, /* in */IEcoLab1 *pIEcoLab1) {
    IEcoConnectionPointContainer* pCPC = 0;
    IEcoConnectionPoint* pCP = 0;
    int16_t result = 0;

    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoConnectionPointContainer, (void**)&pCPC);

    if (result == 0 && pCPC != 0) {
        result = pCPC->pVTbl->FindConnectionPoint(pCPC, &IID_IEcoLab1Events, &pCP);
        pCPC->pVTbl->Release(pCPC);
        pCPC = 0;
        if (result == 0 && pCP != 0) {

            result = pCP->pVTbl->Advise(pCP, (IEcoUnknown*)me, &me->m_cCookie);
            pCP->pVTbl->Release(pCP);
            pCP = 0;
        }
    }

    return result;
}

/*
 *
 * <сводка>
 *   Функция Unadvise
 * </сводка>
 *
 * <описание>
 *   Функция отключения приемника от источника событий
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_Unadvise(/* in */ struct CEcoLab1Sink* me, /* in */IEcoLab1 *pIEcoLab1) {
    IEcoConnectionPointContainer* pCPC = 0;
    IEcoConnectionPoint * pCP = 0;
    int16_t result = 0;

    if (me->m_cCookie) {
        result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoConnectionPointContainer, (void**)&pCPC);
        if (result == 0) {
            result = pCPC->pVTbl->FindConnectionPoint(pCPC, &IID_IEcoLab1Events, &pCP);
            pCPC->pVTbl->Release(pCPC);
            pCPC = 0;
            if (result == 0) {
                result = pCP->pVTbl->Unadvise(pCP, me->m_cCookie);
                pCP->pVTbl->Release(pCP);
                pCP = 0;
            }
        }
    }
    return result;
}

/* Create Virtual Table IEcoLab1VTblEvents */
IEcoLab1VTblEvents g_x2D2E3B9214F248A6A09ECB494B59C795VTblEvents = {
    CEcoLab1Sink_QueryInterface,
    CEcoLab1Sink_AddRef,
    CEcoLab1Sink_Release,
    CEcoLab1Sink_OnBeforeShellSort,
    CEcoLab1Sink_OnAfterShellSort,
    CEcoLab1Sink_ShellSortStep
};

/*
 *
 * <сводка>
 *   Функция Create
 * </сводка>
 *
 * <описание>
 *   Функция создания экземпляра
 * </описание>
 *
 */
int16_t ECOCALLMETHOD createCEcoLab1Sink(/* in */ IEcoMemoryAllocator1* pIMem, /* out */ IEcoLab1Events** ppIEcoLab1Events) {
    int16_t result = -1;
    CEcoLab1Sink* pCMe = 0;

    /* Проверка указателей */
    if (ppIEcoLab1Events == 0 || pIMem == 0 ) {
        return result;
    }

    /* Выделение памяти для данных экземпляра */
    pCMe = (CEcoLab1Sink*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoLab1Sink));

    /* Сохранение указателя на интерфейс для работы с памятью */
    pCMe->m_pIMem = pIMem;
    pCMe->m_pIMem->pVTbl->AddRef(pCMe->m_pIMem);

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Инициализация cookie */
    pCMe->m_cCookie = 0;

    /* Создание таблицы функций интерфейса IEcoLab1Events */
    pCMe->m_pVTblIEcoLab1Events = &g_x2D2E3B9214F248A6A09ECB494B59C795VTblEvents;

    /* Установка вспомогательных функций */
    pCMe->Advise = CEcoLab1Sink_Advise;
    pCMe->Unadvise = CEcoLab1Sink_Unadvise;

    *ppIEcoLab1Events = (IEcoLab1Events*)pCMe;

    return 0;
};

/*
 *
 * <сводка>
 *   Функция Delete
 * </сводка>
 *
 * <описание>
 *   Функция освобождения экземпляра
 * </описание>
 *
 */
void ECOCALLMETHOD deleteCEcoLab1Sink(IEcoLab1Events* pIEcoLab1Events) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*)pIEcoLab1Events;
    IEcoMemoryAllocator1* pIMem = 0;

    if (pIEcoLab1Events != 0 ) {
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
};
