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

void swap(void *arr, size_t i, size_t j, size_t size) {
    char temp[size];
    char *a = (char*)arr;

    memcpy(temp, (a + size * i), size);
    memcpy((a + size * i), (a + size * j), size);
    memcpy((a + size * j), temp, size);
}

size_t buscar_pos_max(void** arr, cmp_func_t cmp, size_t pos_padre, size_t pos_h) {
    if (cmp(arr[pos_padre], arr[pos_h]) > 0) return pos_padre;
    return pos_h;
}

size_t buscar_pos_max_tres (heap_t* heap, size_t pos_padre, size_t pos_h_izq, size_t pos_h_der) {
    size_t largo = heap->cantidad;
    void** arr = heap->arreglo;
    cmp_func_t cmp = heap->cmp;
 
    if (pos_h_izq > largo) return pos_padre; //arbol izq, si noy hay hijo izq no hay der
    if (pos_h_der > largo) return buscar_pos_max(heap->arreglo, cmp, pos_padre, pos_h_izq);

    void* hijo_der = arr[pos_h_der];
    void* hijo_izq = arr[pos_h_izq];
    void* padre = arr[pos_padre];

    if (cmp(hijo_der, hijo_izq) > 0 && cmp(hijo_der, padre) > 0) return pos_h_der;
    if (cmp(hijo_izq, hijo_der) > 0 && cmp(hijo_izq, padre) > 0) return pos_h_izq;
    if (cmp(hijo_der, padre) > 0 && cmp(hijo_der, hijo_izq) == 0) return pos_h_izq;
    return pos_padre;
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
	if(pos == 0) return; // LLegue al inicio del arreglo 

	size_t pos_padre = buscar_pos_padre(pos);

	if(heap->cmp(heap->arreglo[pos], heap->arreglo[pos_padre]) <= 0) return;

	swap(heap->arreglo, pos, pos_padre, sizeof(heap->arreglo[0]));
	upheap(heap, pos_padre);
}

// void downheap(heap_t* heap, size_t pos) {
// 	size_t pos_hijo_der = buscar_pos_hijo_der(pos);
// 	size_t pos_hijo_izq = buscar_pos_hijo_izq(pos);

// 	//Son para que todas las comparaciones queden mucho más legibles
// 	void* elem_act = heap->arreglo[pos];
// 	void* elem_izq;
// 	void* elem_der;

// 	size_t nuevo_act;

// 	if(pos_hijo_izq >= heap->cantidad && pos_hijo_der >= heap->cantidad) return; // No tiene hijos

// 	if(pos_hijo_izq >= heap->cantidad) { // Solo tiene hijo derecho
// 		elem_der = heap->arreglo[pos_hijo_der];
// 		if(heap->cmp(elem_act, elem_der) > 0) return;
// 		nuevo_act = pos_hijo_der;
// 	}
// 	else if(pos_hijo_der >= heap->cantidad) { // Solo tiene hijo izquierdo
// 		elem_izq = heap->arreglo[pos_hijo_izq];
// 		if(heap->cmp(elem_act, elem_izq) > 0) return;
// 		nuevo_act = pos_hijo_izq;
// 	}
// 	else { //Tiene dos hijos
// 		elem_izq = heap->arreglo[pos_hijo_izq];
// 		elem_der = heap->arreglo[pos_hijo_der];
// 		// Si actual es mayor a ambos
// 		if(heap->cmp(elem_act, elem_der) > 0 && heap->cmp(elem_act, elem_izq) > 0) return;
// 		// Si derecho es mayor a ambos
// 		else if(heap->cmp(elem_der, elem_act) > 0 && heap->cmp(elem_der, elem_izq) > 0) nuevo_act = pos_hijo_der;
// 		// Si izquierdo es mayor a ambos
// 		else if(heap->cmp(elem_izq, elem_act) > 0 && heap->cmp(elem_izq, elem_der) > 0) nuevo_act = pos_hijo_izq;
// 		// Si izquierdo y derecho son iguales
// 		else if(heap->cmp(elem_izq, elem_der) == 0) nuevo_act = pos_hijo_izq;
// 		// En otro caso
// 		else return;
// 	}
// 	swap(heap->arreglo, pos, nuevo_act, sizeof(heap->arreglo[0]));
// 	downheap(heap, nuevo_act);
// }

void downheap(heap_t* heap, size_t pos) {
    if (pos >= heap->cantidad - 1) return;
    
    size_t pos_h_der = buscar_pos_hijo_der(pos);
    size_t pos_h_izq = buscar_pos_hijo_izq(pos);

    size_t pos_mayor = buscar_pos_max_tres(heap, pos, pos_h_izq, pos_h_der);

    if (pos_mayor != pos) {
        swap(heap->arreglo, pos, pos_mayor, sizeof(void*));
        downheap(heap, pos_mayor);
    }
}

heap_t* _heap_crear(cmp_func_t cmp) {
    heap_t* heap = malloc(sizeof(heap_t));
    if (!heap) return NULL;

    heap->cmp = cmp;
    heap->cantidad = 0;
    heap->capacidad = CAPACIDAD_INICIAL;

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

    if (!elem) return false;

	heap->arreglo[heap->cantidad] = elem;
	
	upheap(heap, heap->cantidad);
    heap->cantidad++;

    if (heap->capacidad == heap->cantidad) return redimensionar_heap(heap, heap->capacidad * FACTOR_REDIMENSION);
    return true;
}

void *heap_ver_max(const heap_t *heap) {
    return heap->arreglo[0];
}

void *heap_desencolar(heap_t *heap) {
    if (heap->cantidad == 0) return NULL;

    void* elem = heap->arreglo[0];
    heap->arreglo[0] = heap->arreglo[heap->cantidad - 1];

    heap->cantidad--;
    downheap(heap, 0);

    if (heap->cantidad <= heap->capacidad / DISP_DECREMENTO && heap->capacidad > CAPACIDAD_INICIAL) {
        if (!redimensionar_heap(heap, heap->capacidad / FACTOR_REDIMENSION)) return NULL; //no? podria tener error en el medio
    }
    return elem;
}

// void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) { NO SE SI ES PRIMITIVA

// }