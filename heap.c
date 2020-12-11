#include "heap.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h> //Borrar
#include <string.h> //Para Swap

#define CAPACIDAD_INICIAL 20
#define FACTOR_REDIMENSION 2

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

void swap(void *arr, int i, int j, size_t size) {
    char temp[size];
    char *a = (char*)arr;

    memcpy(temp, (a + size * i), size);
    memcpy((a + size * i), (a + size * j), size);
    memcpy((a + size * j), temp, size);
}

// void upheap(heap_t* heap, size_t pos) {
// 	printf("UPHEAP\n");
// 	if(pos == 0) { //es un size_t nunca es menor a 0 
// 		printf("LLegue al inicio del arreglo\n\n");
// 		return; // LLegue al inicio del arreglo
// 	} 

// 	size_t pos_padre = buscar_pos_padre(pos);
// 	printf("pos_padre: %zu, pos_actual: %zu\n", pos_padre, pos);
// 	printf("Padre: %d, Hijo: %d\n", *((int*)heap->arreglo[pos_padre]), *((int*)heap->arreglo[pos]));

// // 	if(pos_padre == pos){ // Creo que nunca entra acá pero está por las dudas, si no sirve la sacamos
// // 		printf("pos_padre invalido\n\n");
// // 		return;
// // 	} 

// 	if(heap->cmp(heap->arreglo[pos], heap->arreglo[pos_padre]) <= 0){
// 		printf("El hijo es menor o igual al pos_padre\n\n");
// 		return;
// 	} 

// 	swap(heap->arreglo, pos, pos_padre, sizeof(heap->arreglo[0]));
	
// 	upheap(heap, pos_padre);
// }

void upheap(heap_t* heap, size_t pos) {
	if(pos == 0) return; // LLegue al inicio del arreglo //es un size_t nunca es menor a 0

	size_t pos_padre = buscar_pos_padre(pos);

	//if(pos_padre == pos) return; //no es necesario creo

	if(heap->cmp(heap->arreglo[pos], heap->arreglo[pos_padre]) <= 0) return;

	swap(heap->arreglo, pos, pos_padre, sizeof(heap->arreglo[0]));
	upheap(heap, pos_padre);
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

	// if(!elem) return false; podria almacenar NULL

	heap->arreglo[heap->cantidad] = elem;
	
	upheap(heap, heap->cantidad);
    heap->cantidad++;

    if (heap->capacidad == heap->cantidad) return redimensior_heap(heap, heap->capacidad * FACTOR_REDIMENSION);
    return true;
}

void *heap_ver_max(const heap_t *heap) {
    return heap->arreglo[0];
}

// void *heap_desencolar(heap_t *heap) {

// }

// void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) { NO SE SI ES PRIMITIVA

// }