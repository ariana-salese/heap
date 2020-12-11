#define _POSIX_C_SOURCE 200809L
#include "heap.h"
#include "lista.h"
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

int cmp_lista(const void* lista1, const void* lista2) {
	// size_t largo1 = lista_largo(lista1);
	// size_t largo2 = lista_largo(lista2);

	// if (largo1 > largo2) return 1;
	// if (largo1 < largo2) return 1;

	return 0;
}

int cmp_str(const void* str1, const void* str2) {
	return strcmp(str1, str2);
}

int cmp_null(const void* null1, const void* null2) {
	return 0;
}

int buscar_mayor(int arr[], size_t n) {
	int mayor = arr[0];

	for (size_t i = 0; i < n; i++) {
		if (arr[i] > mayor) mayor = arr[i];
	}
	return mayor;
}

void _lista_destruir(void* lista) {
	lista_destruir(lista, NULL);
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

static void prueba_crear_heap_vacio() {
	printf("\n> Prueba crear heap vacio:\n");

	heap_t* heap = heap_crear(cmp_ints);

	print_test("Se creo el heap", heap);
	print_test("La cantidad de elementos es 0", heap_cantidad(heap) == 0);
	print_test("ver el mayor devuelve NULL", !heap_ver_max(heap));
	print_test("Desencolar devuelve NULL", !heap_desencolar(heap));

	heap_destruir(heap, NULL);
}

static void prueba_encolar() {
	printf("\n> Prueba encolar algunos elementos:\n");

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

static void prueba_destruir_heap_con_free() {
	printf("\n> Prueba destruir con free\n");

	heap_t* heap = heap_crear(cmp_str);

	char* valores[CANTIDAD_ELEMENTOS];
	char* valores_aux[] = {"pizza", "taco", "lomo", "fideo", "queso", "papa", "lima", "pure", "miel", "crema", "fresa", "nacho", "palta", "arroz", "salsa"};

	/* reserva memoria */
	for (size_t i = 0; i < CANTIDAD_ELEMENTOS; i++) valores[i] = strdup(valores_aux[i]);

	/* encola elementos */
	bool resultado_encolar = true;

	for (size_t i = 0; i < CANTIDAD_ELEMENTOS; i++) {
		if (!heap_encolar(heap, valores[i])) resultado_encolar = false;
	}

	print_test("Se encolaron algunos elementos dinamicos", resultado_encolar);
	print_test("La cantidad de elementos es correcta", heap_cantidad(heap) ==  CANTIDAD_ELEMENTOS);

	heap_destruir(heap, free);
	printf("Se destruyo el heap y sus datos\n");
}

static void prueba_destruir_heap_con_otra_funcion_de_destruccion() {
	printf("\n> Prueba destruir con otra funcion de destruccion\n");

	heap_t* heap = heap_crear(cmp_lista);

	lista_t* listas[CANTIDAD_ELEMENTOS];

	/* crea las listas */
	for (size_t i = 0; i < CANTIDAD_ELEMENTOS; i++) listas[i] = lista_crear();

	/* encola las listas */
	bool resultado_encolar = true;

	for (size_t i = 0; i < CANTIDAD_ELEMENTOS; i++) {
		if (!heap_encolar(heap, listas[i])) resultado_encolar = false;
	}

	print_test("Se encolaron algunos elementos", resultado_encolar);
	print_test("La cantidad de elementos es correcta", heap_cantidad(heap) == CANTIDAD_ELEMENTOS);

	heap_destruir(heap, _lista_destruir);
	printf("Se destruyo el heap y sus datos\n");
}

static void prueba_encolar_null() {
	printf("\n> Prueba encolar NULL\n");

	heap_t* heap = heap_crear(cmp_null);

	/* encola NULLs */
	bool resultado_encolar = true;

	for (size_t i = 0; i < CANTIDAD_ELEMENTOS; i++) {
		if (!heap_encolar(heap, NULL)) resultado_encolar = false;
	}

	print_test("Se encolaron NULLs", resultado_encolar);
	print_test("La cantidad de elementos es correcta", heap_cantidad(heap) == CANTIDAD_ELEMENTOS);
	print_test("Ver maximo devuelve NULL", !heap_ver_max(heap));
	print_test("Desencolar devuelve NULL", !heap_desencolar(heap));
	print_test("Pero se elimino un elemento", heap_cantidad(heap) == CANTIDAD_ELEMENTOS - 1);

	heap_destruir(heap, NULL);
}

void nuestras_pruebas() {
	nuestras_pruebas_marce();
	prueba_upheap();
}

void pruebas_heap_estudiante() {
   	prueba_crear_heap_vacio();
	// prueba_crear_heap_con_arr();
	prueba_encolar();
	// prueba_desencolar(); //estas podriamos metarla en uno o maybe si es repetitivo por lo que hagamos en la de voluemn lo podemos borrar
	prueba_destruir_heap_con_free();
	prueba_destruir_heap_con_otra_funcion_de_destruccion();
	prueba_encolar_null();
	// prueba_de_volumen();
	// prueba_heap_sort();
}

// //#ifndef CORRECTOR

// int main(void) {
//     pruebas_heap_estudiante();
//     return failure_count() > 0;
// }

// //#endif
