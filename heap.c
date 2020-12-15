#include "heap.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h> //Borrar
#include <string.h> //Para Swap

#define CAPACIDAD_INICIAL 20
#define FACTOR_REDIMENSION 2
#define DISP_DECREMENTO 4 //DISP por "disparador" o "trigger"

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

void imprimir_heap_size_t(heap_t* heap) {
    printf("[");
    for (size_t i = 0; i < heap->cantidad; i++) {
        printf("%zu", *(size_t*)heap->arreglo[i]);
        if (i != heap->cantidad - 1) printf(", ");
    }
    printf("]\n");
}

size_t capacidad(heap_t* heap) {
    return heap->capacidad;
}

void imprimir_arr_int(void *arr[], size_t n) {
    printf("[");
    for (size_t i = 0; i < n; i++) {
        printf("%i", *(int*)arr[i]);
        if (i != n - 1) printf(", ");
    }
    printf("]\n");
}

/* ******************************************************************
 *                       FUNCIONES AUXILIARES
 * *****************************************************************/

size_t buscar_pos_padre (size_t pos_hijo) {
    return (pos_hijo - 1) / 2;
}

size_t buscar_pos_hijo_der (size_t pos_padre) {
    return 2 * pos_padre + 2;
}

size_t buscar_pos_hijo_izq (size_t pos_padre) {
    return 2 * pos_padre + 1;
}

void swap(void **x, void** y) {
    void* aux = *x;
    *x = *y;
    *y = aux;
}

size_t buscar_pos_max_tres (void* arr[], cmp_func_t cmp, size_t largo, size_t pos_padre, size_t pos_h_izq, size_t pos_h_der) {
 	
    if (pos_h_izq >= largo) return pos_padre; //arbol izq, si noy hay hijo izq no hay der
    if (pos_h_der >= largo) return cmp(arr[pos_padre], arr[pos_h_izq]) > 0 ? pos_padre : pos_h_izq;

    void* hijo_der = arr[pos_h_der];
    void* hijo_izq = arr[pos_h_izq];
    void* padre = arr[pos_padre];

    if (cmp(hijo_der, hijo_izq) > 0 && cmp(hijo_der, padre) > 0) return pos_h_der;
    if (cmp(hijo_izq, hijo_der) > 0 && cmp(hijo_izq, padre) > 0) return pos_h_izq;
    if (cmp(hijo_der, padre) > 0 && cmp(hijo_der, hijo_izq) == 0) return pos_h_izq;
    return pos_padre;
}

void upheap(heap_t* heap, size_t pos) {
	if(pos == 0) return; // LLegue al inicio del arreglo 

	size_t pos_padre = buscar_pos_padre(pos);

	if(heap->cmp(heap->arreglo[pos], heap->arreglo[pos_padre]) <= 0) return;

	swap(&heap->arreglo[pos], &heap->arreglo[pos_padre]);
	upheap(heap, pos_padre);
}

void downheap(void* arr[], size_t largo, cmp_func_t cmp, size_t pos) {
    if (pos >= largo - 1) return;
    
    size_t pos_h_der = buscar_pos_hijo_der(pos);
    size_t pos_h_izq = buscar_pos_hijo_izq(pos);

    size_t pos_mayor = buscar_pos_max_tres(arr, cmp, largo, pos, pos_h_izq, pos_h_der);

    if (pos_mayor != pos) {
        swap(&arr[pos], &arr[pos_mayor]);
        downheap(arr, largo, cmp, pos_mayor);
    }
}

void heapify(void *elementos[], size_t largo, cmp_func_t cmp) {
	for (size_t i = largo; i > 0; i--) {	
		downheap(elementos, largo, cmp, i - 1);
	}
}

heap_t* _heap_crear(cmp_func_t cmp, size_t capacidad) {
    heap_t* heap = malloc(sizeof(heap_t));
    if (!heap) return NULL;

    void** arreglo = malloc(sizeof(void*) * capacidad);
    if (!arreglo) {
        free(heap);
        return NULL;
    }

    heap->cmp = cmp;
    heap->cantidad = 0;
    heap->capacidad = capacidad;
    heap->arreglo = arreglo;

    return heap;
}

bool redimensionar_heap(heap_t* heap, size_t nueva_capacidad) { 
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
    heap_t* heap =_heap_crear(cmp, CAPACIDAD_INICIAL);
    if (!heap) return NULL;

    return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
    size_t capacidad = CAPACIDAD_INICIAL;
    if (n == capacidad) capacidad = CAPACIDAD_INICIAL * FACTOR_REDIMENSION;
    if (n > capacidad * 2) capacidad = CAPACIDAD_INICIAL * FACTOR_REDIMENSION * n;

	heap_t* heap =_heap_crear(cmp, capacidad);
	if (!heap) return NULL;

	heapify(arreglo, n, cmp);

	for (size_t i = 0; i < n; i++) {
        heap->arreglo[i] = arreglo[i];
    }

	heap->cantidad = n;

	return heap;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)) {
    void** arr = heap->arreglo;

    if (destruir_elemento) {
        for (size_t i = 0; i < heap->cantidad; i++) {
            destruir_elemento(arr[i]);
        }
    }
    free(arr);
    free(heap);
}

size_t heap_cantidad(const heap_t *heap) {
    return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap) {
    return heap->cantidad == 0;
}

bool heap_encolar(heap_t *heap, void *elem) {
    if (!elem) return false;

	heap->arreglo[heap->cantidad] = elem;
	
	upheap(heap, heap->cantidad);
    heap->cantidad++;

    if (heap->capacidad == heap->cantidad) return redimensionar_heap(heap, heap->capacidad * FACTOR_REDIMENSION);
    return true;
}

void *heap_ver_max(const heap_t *heap) {
    if (heap->cantidad != 0) return heap->arreglo[0];
    return NULL;
}

void *heap_desencolar(heap_t *heap) {
    if (heap->cantidad == 0) return NULL;

    void* elem = heap->arreglo[0];
    heap->arreglo[0] = heap->arreglo[heap->cantidad - 1];

    heap->cantidad--;
    downheap(heap->arreglo, heap->cantidad, heap->cmp, 0);

    if (heap->cantidad <= heap->capacidad / DISP_DECREMENTO && heap->capacidad > CAPACIDAD_INICIAL) {
        if (!redimensionar_heap(heap, heap->capacidad / FACTOR_REDIMENSION)) return NULL; 
    }
    return elem;
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) { 
    heapify(elementos, cant, cmp);
    size_t largo = cant - 1;

    for (size_t i = 0; i < cant; i++, largo--) {
        swap(&elementos[0], &elementos[largo]);
        downheap(elementos, largo, cmp, 0);
    }
}