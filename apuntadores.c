/*
 * Ejemplo básico de apuntadores en C
 *
 * Compilar:  gcc -Wall -o apuntadores apuntadores.c
 * Ejecutar:  ./apuntadores
 */
#include <stdio.h>

/* Recibe las direcciones de dos variables e intercambia sus valores */
void intercambiar(int *a, int *b) {
    int temp = *a;   /* *a = valor guardado en la dirección a */
    *a = *b;
    *b = temp;
}

int main(void) {
    /* 1. Declarar una variable y un apuntador a ella */
    int numero = 10;
    int *ptr = &numero;   /* & obtiene la dirección de 'numero' */

    printf("=== 1. Conceptos basicos ===\n");
    printf("Valor de numero:          %d\n", numero);
    printf("Direccion de numero:      %p\n", (void *)&numero);
    printf("Valor de ptr (direccion): %p\n", (void *)ptr);
    printf("Valor apuntado (*ptr):    %d\n", *ptr);

    /* 2. Modificar la variable a través del apuntador */
    *ptr = 25;
    printf("\n=== 2. Modificar mediante el apuntador ===\n");
    printf("Despues de *ptr = 25, numero vale: %d\n", numero);

    /* 3. Paso por referencia a una función */
    int x = 3, y = 7;
    printf("\n=== 3. Paso por referencia ===\n");
    printf("Antes:   x = %d, y = %d\n", x, y);
    intercambiar(&x, &y);
    printf("Despues: x = %d, y = %d\n", x, y);

    /* 4. Apuntadores y arreglos (aritmética de apuntadores) */
    int arreglo[5] = {1, 2, 3, 4, 5};
    int *p = arreglo;     /* el nombre del arreglo es la dirección de su primer elemento */

    printf("\n=== 4. Apuntadores y arreglos ===\n");
    for (int i = 0; i < 5; i++) {
        printf("arreglo[%d] = %d  |  *(p + %d) = %d  |  direccion: %p\n",
               i, arreglo[i], i, *(p + i), (void *)(p + i));
    }

    /* 5. Apuntador a apuntador */
    int **pp = &ptr;
    printf("\n=== 5. Apuntador a apuntador ===\n");
    printf("**pp = %d (mismo valor que numero)\n", **pp);

    /* 6. Apuntador nulo */
    int *nulo = NULL;
    printf("\n=== 6. Apuntador NULL ===\n");
    if (nulo == NULL) {
        printf("El apuntador es NULL: no se debe desreferenciar.\n");
    }

    return 0;
}
