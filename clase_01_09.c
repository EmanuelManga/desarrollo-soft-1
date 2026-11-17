#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct alumno{
    int legajo;
    char nombre[100];
    char apellido[100];
    float promedio;
} Alumno;

int sumar(int , int);
void clase_char(void);
int clase_suma(void);
void clase_struct(void);
void clase_salir(void);
void clase_punteros(void);
void clase_malloc(void);
int bubble_sort(int *, int);
int clase_malloc_struct(void);
void limpiar_pantalla(void);
int guardar_datos_en_archivo(char *, Alumno *, int);
int main(void){
    int opcion;
    printf("Menu de opciones:\n");
    printf("1. Suma\n");
    printf("2. CHAR\n");
    printf("3. STRUCT\n");
    printf("4. PUNTEROS\n");
    printf("5. MALLOC\n");
    printf("6. MALLOC STRUCT\n");
    printf("20. Salir\n");
    printf("\n Ingrese la opcion que desea realizar:");
    scanf("%d", &opcion);
    limpiar_pantalla();
    switch (opcion) {
        case 1:
            clase_suma();
            break;
        case 2:
            clase_char();
            break;
        case 3:
            clase_struct();
            break;
        case 4:
            clase_punteros();
            break;
        case 5:
            clase_malloc();
            break;
        case 6:
            clase_malloc_struct();
            break;
        case 20:
            break;
        default:
            printf("Opcion invalida\n");
            break;
    }
    clase_salir();
    return 0;
}

int clase_suma(void){
    int val1, val2, resultado;
    printf("Ingrese el primer valor: ");
    scanf("%d", &val1);
    while (val1 < 0) {
        printf("El valor ingresado es invalido\n");
        printf("Ingrese el primer valor: ");
        scanf("%d", &val1);
    }
    printf("Ingrese el segundo valor: ");
    scanf("%d", &val2);
    while (val2 < 0) {
        printf("El valor ingresado es invalido\n");
        printf("Ingrese el segundo valor: ");
        scanf("%d", &val2);
    }
    resultado = sumar(val1, val2);
    printf("El resultado de la suma es: %d\n", resultado);
    return resultado;
}

int sumar(int a, int b){
    return a + b;
}

void clase_char(void){
    char nombre[100];
    printf("Ingrese su nombre: ");
    while (getchar() != '\n');

    fgets(nombre, 100, stdin);
    fputs(nombre, stdout);
}

void clase_struct(void){
    Alumno alumno1;
    printf("\n Ingrese el legajo del alumno: ");
    scanf("%d", &alumno1.legajo);
    while (getchar() != '\n')
        ;
    printf("\n Ingrese el nombre del alumno: ");
    fgets(alumno1.nombre, 100, stdin);
    printf("\n Ingrese el apellido del alumno: ");
    fgets(alumno1.apellido, 100, stdin);
    printf("\n Ingrese el promedio del alumno: ");
    scanf("%f", &alumno1.promedio);
    printf("\n El legajo del alumno es: %d\n", alumno1.legajo);
    printf("\n El nombre del alumno es: ");
    fputs(alumno1.nombre, stdout);
    printf("\n El apellido del alumno es: ");
    fputs(alumno1.apellido, stdout);
    printf("\n El promedio del alumno es: %.2f", alumno1.promedio);
}

void clase_punteros(void){
    int *puntero;
    int valor = 10;
    puntero = &valor;
    printf("\n El valor del puntero es: %d", *puntero);
}

void clase_malloc(void){
    int *puntero;
    int i,tam=10;
    char opcion;
    puntero = (int *)malloc(sizeof(int)*tam);
    if (puntero == NULL) {
        printf("Error al asignar memoria\n");
        return;
    }
    for( i = 0; i < tam; i++){
        printf("\n Ingrese el valor %d: ", i+1);
        scanf("%d", &puntero[i]);
    }
    for( i = 0; i < tam; i++){
        printf("\n El valor %d es: %d", i+1, puntero[i]);
    }
    
    printf("\n Desea ordenar los valores? (s/n): ");
    while (getchar() != '\n');
    scanf("%c", &opcion);
    if(opcion == 's'){
        bubble_sort(puntero, tam);
        for( i = 0; i < tam; i++){
            printf("\n El valor %d es: %d", i+1, puntero[i]);
        }
    }
    
    free(puntero);
}

int clase_malloc_struct(void){
    char nombre_archivo[100];
    Alumno *puntero;
    int i,tam=3;
    puntero = (Alumno *)malloc(sizeof(Alumno)*tam);
    if (puntero == NULL) {
        printf("Error al asignar memoria\n");
        return 0;
    }
    for( i = 0; i < tam; i++){
        printf("Ingrese el legajo del alumno: ");
        scanf("%d", &puntero[i].legajo);
        printf("Ingrese el nombre del alumno: ");
        while (getchar() != '\n');
        fgets(puntero[i].nombre, 100, stdin);
        printf("Ingrese el apellido del alumno: ");
        fgets(puntero[i].apellido, 100, stdin);
        printf("Ingrese el promedio del alumno: ");
        scanf("%f", &puntero[i].promedio);
        limpiar_pantalla();
    }
    for( i = 0; i < tam; i++){
        printf("\n Alumno %d:", i+1);
        printf("\n El legajo del alumno es: %d", puntero[i].legajo);
        printf("\n El nombre del alumno es: %s", puntero[i].nombre);
        printf(" El apellido del alumno es: %s", puntero[i].apellido);
        printf(" El promedio del alumno es: %.2f", puntero[i].promedio);
        printf("\n\n--------------------o--------------------\n");
    }

    printf("Ingrese el nombre del archivo: ");
    scanf("%s", nombre_archivo);
    guardar_datos_en_archivo(nombre_archivo, puntero, tam);

    free(puntero);
    return 0;
}

void clase_salir(void){
    printf("\nPresione Enter para salir\n");
    while (getchar() != '\n');
    getchar();

}

int bubble_sort(int *puntero, int tam){
    int i, j, aux;
    for(i = 0; i < tam-1; i++){
        for(j = 0; j < tam-i-1; j++){
            if(puntero[j] > puntero[j+1]){
                aux = puntero[j];
                puntero[j] = puntero[j+1];
                puntero[j+1] = aux;
            }
        }
    }
    return 0;
}

void limpiar_pantalla(void){
    system("clear");
    return;
}


int guardar_datos_en_archivo(char *nombre_archivo, Alumno *puntero, int tam){
    int i;
    FILE *archivo;
    archivo = fopen(nombre_archivo, "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return 0;
    }
    for( i = 0; i < tam; i++){
        fprintf(archivo, "%d %.2f\n", puntero[i].legajo,  puntero[i].promedio);
    }
    fclose(archivo);
    return 0;
}