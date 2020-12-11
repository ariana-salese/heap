#define _POSIX_C_SOURCE 200809L
#include "heap.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CANTIDAD_ELEMENTOS 15

/* ******************************************************************
 *                 NUESTRAS PRUEBAS (ELIMINAR)
 * *****************************************************************/

int cmp_ints(const void *a, const void *b) {

	int value_a = *((int*)a);
	int value_b = *((int*)b);

	if(value_a < value_b) return -1;
	if(value_a > value_b) return 1;
	return 0;
}

void nuestras_pruebas() {

	int elems[] = {5,7,8,10,6,2,1,0,5,4,3,9};

	heap_t* heap = heap_crear(cmp_ints);

	for(int i = 0; i < 12; i++){
		heap_encolar(heap, elems+i);
		imprimir_heap_int(heap);
		printf("\n\n");
	}
}
/* ******************************************************************
 *                 PRUEBAS PRIMITIVAS HEAP
 * *****************************************************************/

void pruebas_heap_estudiante() {
    nuestras_pruebas();
}

//#ifndef CORRECTOR

int main(void) {
    pruebas_heap_estudiante();
    return failure_count() > 0;
}

//#endif
