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

void swap_size_t(size_t* x, size_t* y) {
    size_t aux = *x;
    *x = *y;
    *y = aux;
}

void ordenar_arr(size_t* arr, size_t largo) {
	size_t pos_act = largo;

	for (size_t i = 0; i < largo && pos_act != 0; i++) {
		if (arr[pos_act] > arr[pos_act - 1]) {
			swap_size_t(&arr[pos_act], &arr[pos_act - 1]);
			pos_act--;
		}
		else return;
	}
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

static void prueba_crear_heap_con_arr() {
	printf("\n> Prueba crear heap con arreglo:\n");

	int aux[] = {5, 0, 8, 9, 6, 2, 1, 7, 5, 4, 3, 10, 2, 14, -4};
	void* arreglo[CANTIDAD_ELEMENTOS];

	for (size_t i = 0; i < CANTIDAD_ELEMENTOS; i++) arreglo[i] = &aux[i];

	size_t cantidad = (size_t)CANTIDAD_ELEMENTOS;

	heap_t* heap = heap_crear_arr(arreglo, cantidad, cmp_ints);

	print_test("Se creo el heap", heap);
	print_test("La cantidad de elementos es la del arrelgo", heap_cantidad(heap) == CANTIDAD_ELEMENTOS);
	print_test("Ver maximo", *(int*)heap_ver_max(heap) == 14);
	print_test("Desencolar devuelve el maximo", *(int*)heap_desencolar(heap) == 14);
	print_test("Ver maximo es el proximo mas grande", *(int*)heap_ver_max(heap) == 10);
	print_test("Desencolar devuelve el maximo", *(int*)heap_desencolar(heap) == 10);
	print_test("La cantidad de elementos es correcta", heap_cantidad(heap) == CANTIDAD_ELEMENTOS - 2);

	heap_destruir(heap, NULL);
}

static void prueba_uso_completo() {
	printf("\n> Prueba uso completo:\n");

	int arr_ini_aux[] = {3, 5, 1, 1, 1, 6, 3, -7};
	int arr_enc[] = {8, 2, 15, 10, 2, 9, -4};
	int arr_orden[] = {15, 10, 9, 8, 6, 5, 3, 3, 2, 2, 1, 1, 1, -4, -7};

	void* arr_ini[CANTIDAD_ELEMENTOS / 2 + 1]; 
	for (size_t i = 0; i < CANTIDAD_ELEMENTOS / 2 + 1; i++) arr_ini[i] = arr_ini_aux + i;

	size_t cantidad = (size_t)CANTIDAD_ELEMENTOS;

	heap_t* heap = heap_crear_arr(arr_ini, cantidad / 2 + 1, cmp_ints);

	print_test("Se creo el heap", heap);
	print_test("La cantidad de elementos es correcta", heap_cantidad(heap) == cantidad / 2 + 1);
	print_test("El máximo es el esperado", *((int*)heap_ver_max(heap)) == 6);

	print_test("La cantidad de elementos sigue siendo la misma", heap_cantidad(heap) == cantidad / 2 + 1);
	print_test("El máximo es el mismo", *(int*)heap_ver_max(heap) == 6);

	bool resultado_encolar = true;
	bool resultado_cantidad = true;
	
	for (size_t i = 0; i < cantidad / 2 && resultado_encolar; i++) {
		if (!heap_encolar(heap, arr_enc + i)) resultado_encolar = false;
		if (heap_cantidad(heap) != cantidad / 2 + i + 2) resultado_cantidad = false;
	}

	print_test("Se encolaron varios elementos", resultado_encolar);
	print_test("La cantidad de elementos se actualizo correctamente", resultado_cantidad);
	print_test("El máximo es el esperado", *(int*)heap_ver_max(heap) == arr_orden[0]);

	printf("Desencolo la mitad de los elementos\n");

	bool resultado_desencolar = true;
	resultado_cantidad = true;

	for (size_t i = 0; i < cantidad / 2; i++) {

		if (*(size_t*)heap_desencolar(heap) != arr_orden[i]) resultado_desencolar = false;
		if (heap_cantidad(heap) != cantidad - i - 1) resultado_cantidad = false;
	}

	print_test("Se desencolaron la mitad de los elementos", resultado_desencolar);
	print_test("La cantidad de elementos se actualizo correctamente", resultado_cantidad);
	print_test("El máximo es el esperado", *(int*)heap_ver_max(heap) == arr_orden[cantidad / 2]);

	printf("Destruyo el heap\n");
	heap_destruir(heap, NULL);
}

static void prueba_encolar_null() {
	printf("\n> Prueba encolar NULL:\n");

	heap_t* heap = heap_crear(cmp_str);

	print_test("Encolar NULL devuelve false", !heap_encolar(heap, NULL));
	print_test("La cantidad de elementos es 0", heap_cantidad(heap) == 0);

	heap_destruir(heap, NULL);
}


static void prueba_destruir_heap_con_free() {
	printf("\n> Prueba destruir con free:\n");

	heap_t* heap = heap_crear(cmp_str);

	char* valores[CANTIDAD_ELEMENTOS];
	char* valores_aux[] = {"13", "06", "10", "05", "03", "15", "19", "12", "20", "07", "09", "11", "33", "87", "06"};

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
	printf("\n> Prueba destruir con otra funcion de destruccion:\n");

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

void _prueba_de_volumen(size_t cantidad_elementos) {

	heap_t* heap = heap_crear(cmp_ints);
	size_t valores[cantidad_elementos];
	size_t valores_en_orden[cantidad_elementos];

	// ENCOLA ELEMENTOS

	/* pruebas */
	bool resultado_encolar = true;
	bool resultado_ver_max = true;
	bool resultado_cantidad = true;

	for (size_t i = 0; i < cantidad_elementos; i++) {
		valores[i] = rand();
		valores_en_orden[i] = valores[i];
		ordenar_arr(valores_en_orden, i);

		if (!heap_encolar(heap, &valores[i])) resultado_encolar = false;
		if (*(size_t*)heap_ver_max(heap) != valores_en_orden[0]) resultado_ver_max  = false;
		if (heap_cantidad(heap) != i + 1) resultado_cantidad = false;

	}

	print_test("Se encolaron todos lo elementos", resultado_encolar);
	print_test("Ver el maximo del heap siempre devuelve el mayor", resultado_ver_max);
	print_test("La cantidad de elementos se actualizo correctamente", resultado_cantidad);

	// DESENCOLA MITAD DE ELEMENTOS
	
	/* pruebas */
	bool resultado_desencolar = true;
	bool resultado_ver_max_pre = true;
	bool resultado_ver_max_post = true;
	resultado_cantidad = true;

	for (size_t i = 0; i < cantidad_elementos / 2; i++) {

		if (*(size_t*)heap_ver_max(heap) != valores_en_orden[i]) resultado_ver_max_pre = false;
		if (*(size_t*)heap_desencolar(heap) != valores_en_orden[i]) resultado_desencolar = false;
		if (*(size_t*)heap_ver_max(heap) != valores_en_orden[i + 1]) resultado_ver_max_post = false;
		if (heap_cantidad(heap) != cantidad_elementos - i - 1) resultado_cantidad = false;
	}

	print_test("Ver maximo devuelve el elemento que se desencolara", resultado_ver_max_pre);
	print_test("Desencolar la mitad de los elementos", resultado_desencolar);
	print_test("Ver el maximo luego de desencolar devuelve el proximo mayor", resultado_ver_max_post);
	print_test("La cantidad de elementos se actualizo correctamente", resultado_cantidad);

	/* destruye el heap con la mitad de los elementos encolado */
	heap_destruir(heap, NULL);
}

void _prueba_de_volumen_con_arr(size_t cantidad_elementos) {

	size_t valores_en_orden[cantidad_elementos];
	void* valores[cantidad_elementos];
	size_t valores_aux[cantidad_elementos];


	/* inicializa valores random en valores_arr */
	for (size_t i = 0; i < cantidad_elementos / 2; i++) {
		valores_aux[i] = rand();
		valores_en_orden[i] = valores_aux[i];
		valores[i] = &valores_aux[i];

		ordenar_arr(valores_en_orden, i);
	}

	//CREAR HEAP CON ARR

	heap_t* heap = heap_crear_arr(valores, cantidad_elementos / 2, cmp_ints);

	print_test("El heap se creo", heap);
	print_test("La cantidad de elementos es igual a la del arreglo inicial", heap_cantidad(heap) == cantidad_elementos / 2);

	//ENCOLA, la otra mitad de los elementos 

	/* pruebas */
	bool resultado_encolar = true;
	bool resultado_cantidad = true;
	bool resultado_ver_max = true;

	for (size_t i = cantidad_elementos / 2; i < cantidad_elementos; i++) {

		valores_aux[i] = rand();
		valores_en_orden[i] = valores_aux[i];
		valores[i] = &valores_aux[i];

		ordenar_arr(valores_en_orden, i);

		if (!heap_encolar(heap, valores[i])) resultado_encolar = false;
		if (heap_cantidad(heap) != i + 1) resultado_cantidad = false;
		if (*(int*)heap_ver_max(heap) != valores_en_orden[0]) resultado_ver_max = false;
	}

	print_test("Se encolo la otra mitad de lo elementos", resultado_encolar);
	print_test("Ver el maximo del heap siempre devuelve el mayor", resultado_ver_max);
	print_test("La cantidad de elementos se actualizo correctamente", resultado_cantidad);

	// DESENCOLA TODOS
	
	/* pruebas */
	bool resultado_desencolar = true;
	bool resultado_ver_max_pre = true;
	bool resultado_ver_max_post = true;
	resultado_cantidad = true;

	for (size_t i = 0; i < cantidad_elementos; i++) {

		if (*(size_t*)heap_ver_max(heap) != valores_en_orden[i]) resultado_ver_max_pre = false;
		if (*(size_t*)heap_desencolar(heap) != valores_en_orden[i]) resultado_desencolar = false;
		if (i != cantidad_elementos - 1) if (*(size_t*)heap_ver_max(heap) != valores_en_orden[i + 1]) resultado_ver_max_post = false;
		if (heap_cantidad(heap) != cantidad_elementos - i - 1) resultado_cantidad = false;
	}

	print_test("Ver maximo devuelve el elemento que se desencolara", resultado_ver_max_pre);
	print_test("Desencolar todos los elementos", resultado_desencolar);
	print_test("Ver el maximo luego de desencolar siempre devuelve el proximo mayor", resultado_ver_max_post);
	print_test("La cantidad de elementos se actualizo correctamente", resultado_cantidad);
	print_test("La cantidad de elementos es 0", heap_cantidad(heap) == 0);

	/* destruye el heap */
	heap_destruir(heap, NULL);
}

static void prueba_de_volumen() {

	printf("\n> Prueba de volumen con arreglo vacio (5000 elementos)\n");
	_prueba_de_volumen(5000);

	printf("\n> Prueba de volumen con arreglo vacio (10000 elementos)\n");
	_prueba_de_volumen(10000);

	printf("\n> Prueba de volumen con arreglo inicial(5000 elementos)\n");
	_prueba_de_volumen_con_arr(5000);

	printf("\n> Prueba de volumen con arreglo inicial(10000 elementos)\n");
	_prueba_de_volumen_con_arr(10000);
}

static void prueba_heap_sort() {
	printf("\n> Prueba heap sort:\n");

	size_t cantidad_elementos = 1000;
	size_t arr_aux[cantidad_elementos];
	void* arr[cantidad_elementos];
	size_t arr_en_orden[cantidad_elementos];
	bool resultado_orden = true;

	for (size_t i = 0; i < cantidad_elementos; i++) {
		arr_aux[i] = rand();
		arr_en_orden[i] = arr_aux[i];
		arr[i] = &arr_aux[i];

		ordenar_arr(arr_en_orden, i);
	}

	heap_sort(arr, cantidad_elementos, cmp_ints);
	printf("Se aplico heapsort\n");

	for (size_t i = 0, j = cantidad_elementos - 1; i < cantidad_elementos; i++, j--) {
		if (*(int*)arr[i] != arr_en_orden[j]) resultado_orden = false;
	}

	print_test("EL arreglo se encuentra en orden", resultado_orden);
}

void pruebas_heap_estudiante() {
   	prueba_crear_heap_vacio();
	prueba_crear_heap_con_arr();
	prueba_uso_completo();
	prueba_encolar_null();
	prueba_destruir_heap_con_free();
	prueba_destruir_heap_con_otra_funcion_de_destruccion();
	prueba_de_volumen();
	prueba_heap_sort();
}

