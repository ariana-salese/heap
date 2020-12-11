#define _POSIX_C_SOURCE 200809L
#include "heap.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CANTIDAD_ELEMENTOS 15

/* ******************************************************************
 *                       FUNCIONES AUXILIARES
 * *****************************************************************/

int cmp_ints(const void *a, const void *b) {

	int value_a = *((int*)a);
	int value_b = *((int*)b);

	if(value_a < value_b) return -1;
	if(value_a > value_b) return 1;
	return 0;
}

int buscar_mayor(int arr[], size_t n) {
	int mayor = arr[0];

	for (size_t i = 0; i < n; i++) {
		if (arr[i] > mayor) mayor = arr[i];
	}
	return mayor;
}

/* ******************************************************************
 *                 NUESTRAS PRUEBAS (ELIMINAR)
 * *****************************************************************/

void imprimir_arreglo_int(int* arr, size_t largo) {
    printf("[");
    for (size_t i = 0; i < largo; i++) {
        printf("%i", arr[i]);
        if (i != largo - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

void nuestras_pruebas_marce() {

	int arr[] = {5, 0, 8, 9, 6, 2, 1, 7, 5, 4, 3, 10, 2, 14, -4};
	//int arr_final[] = {14, 8, 10, 7, 6, 5, 9, 0, 5, 4, 3, 2, 2, 1, -4}; //lo verifique a mano asi que lo podemos usar pa las pruebas  

	heap_t* heap = heap_crear(cmp_ints);

	for(int i = 0; i < CANTIDAD_ELEMENTOS; i++){
		heap_encolar(heap, arr + i);
		imprimir_heap_int(heap);
		printf("\n\n");
	}

}

static void prueba_upheap() {
    printf("\nPRUEBA UPHEAP:\n");
    heap_t* heap = heap_crear(cmp_ints);

    print_test("Se creo el heap", heap);
    print_test("La cantidad de elementos es 0", heap_cantidad(heap) == 0);
    int arr[] = {7, 5, 6, 4, 0, -3, 4, 2}; //cola de prioridad con orden final correcto

    size_t largo = 8;

    for (size_t i = 0; i < largo; i++)  heap_encolar(heap, &arr[i]);

    imprimir_heap_int(heap); //no deberia modificarse

    int enc1 = 3;
    heap_encolar(heap, &enc1);
    imprimir_heap_int(heap); //{7, 5, 6, 4, 0, -3, 4, 2, 3}

    int enc2 = 6;
    heap_encolar(heap, &enc2);
    imprimir_heap_int(heap); //{7, 6, 6, 4, 5, -3, 4, 2, 3, 0};
}

/* ******************************************************************
 *                 PRUEBAS PRIMITIVAS HEAP
 * *****************************************************************/

static void prueba_encolar() {
	printf("\nprueba encolar algunos elementos:\n");

	heap_t* heap = heap_crear(cmp_ints);

	print_test("Se creo el heap", heap);

	int arr[] = {5, 0, 8, 9, 6, 2, 1, 7, 5, 4, 3, 10, 2, 14, -4};
	//int arr_final[] = {14, 8, 10, 7, 6, 5, 9, 0, 5, 4, 3, 2, 2, 1, -4};

	bool resultado_encolar = true;
	bool resultado_mayor = true;
	bool resultado_cantidad = true;

	for (size_t i = 0; i < CANTIDAD_ELEMENTOS && resultado_encolar; i++) {
		if (!heap_encolar(heap, &arr[i])) resultado_encolar = false;
		if (*(int*)heap_ver_max(heap) != buscar_mayor(arr, i + 1)) resultado_mayor = false;
		if (heap_cantidad(heap) != i + 1) resultado_cantidad = false;
	}

	print_test("Se encolaron varios elementos", resultado_encolar);
	print_test("La cantidad de elementos se actualizo correctamente", resultado_cantidad);
	print_test("El mayor siempre es el primero del heap", resultado_mayor);
	
	heap_destruir(heap, NULL);
}

void nuestras_pruebas() {
	nuestras_pruebas_marce();
	prueba_upheap();
}

void pruebas_heap_estudiante() {
    // prueba_crear_heap_vacio();
	// prueba_crear_heap_con_arr();
	prueba_encolar();
	// prueba_desencolar();
	// prueba_destruir_heap_con_funcion_de_destruccion();
	// prueba_de_volumen();
	// prueba_con_null();
	// prueba_heap_sort();
}

// //#ifndef CORRECTOR

// int main(void) {
//     pruebas_heap_estudiante();
//     return failure_count() > 0;
// }

// //#endif
