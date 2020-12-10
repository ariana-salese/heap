#include "heap.h"
#include <stdbool.h>
#include <stdlib.h>

#define CAPACIDAD_INICIAL 20

struct heap {
    void** arreglo;
    size_t cantidad;
    size_t capacidad;
    cmp_func_t cmp;
};

/* ******************************************************************
 *                              (ELIMINAR)
 * *****************************************************************/

void imprimir_heap_int(heap_t* heap) {
    printf("[");
    for (size_t i = 0; i < heap->cantidad; i++) {
        printf("%i", *(int*)heap->arreglo[i]);
        if (i != heap->cantidad - 1) printf(", ");
    }
    printf("]\n");
}

size_t capacidad(heap_t* heap) {
    return heap->capacidad;
}
//las use para debugear por ahi te sirven 

/* ******************************************************************
 *                       FUNCIONES AUXILIARES
 * *****************************************************************/

size_t buscar_pos_padre (size_t pos_hijo) {
    return (pos_hijo - 1);
}

size_t buscar_pos_hijo_der (size_t pos_padre) {
    return 2 * (pos_padre + 2);
}

size_t buscar_pos_hijo_izq (size_t pos_padre) {
    return 2 * (pos_padre + 1);
}

heap_t* _heap_crear(cmp_func_t cmp) {
    heap_t* heap = malloc(sizeof(heap_t));
    if (!heap) return NULL;

    heap->cmp = cmp;
    heap->cantidad = 0;
    heap->capacidad = CAPACIDAD_INICIAL;

    return heap;
}

bool redimensior_heap(heap_t* heap, size_t nueva_capacidad) {
    void** nuevo_arr = realloc(heap->arreglo, sizeof(void*) * nueva_capacidad);
    if (!nuevo_arr) return false;

    heap->capacidad = nueva_capacidad;
    heap->arreglo = nuevo_arr;

    return true;
}

/* ******************************************************************
 *                          PRIMITIVAS HEAP
 * *****************************************************************/

heap_t *heap_crear(cmp_func_t cmp) {
    heap_t* heap =_heap_crear(cmp);
    if (!heap) return NULL;

    void** arreglo = malloc(sizeof(void*) * CAPACIDAD_INICIAL);
    if (!arreglo) {
        free(heap);
        return NULL;
    }

    heap->arreglo = arreglo;

    return heap;
}

// heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
//     heap_t* heap =_heap_crear(cmp);
//     if (!heap) return NULL;

// }

// void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)) {

// }

size_t heap_cantidad(const heap_t *heap) {
    return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap) {
    return heap->cantidad == 0;
}


// bool heap_encolar(heap_t *heap, void *elem) {

// }

void *heap_ver_max(const heap_t *heap) {
    return heap->arreglo[0];
}

// void *heap_desencolar(heap_t *heap) {

// }

// void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) { NO SE SI ES PRIMITIVA

// }
