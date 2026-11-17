#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_OPCIONES 8

#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h>
    #define LIMPIAR_PANTALLA "cls"
    #define ESPACIO 32
    #define ENTER 13

    int leer_tecla(void) {
        int tecla = _getch();
        if (tecla == 0 || tecla == 224) {
            tecla = _getch();
            if (tecla == 72) return 1;
            if (tecla == 80) return 2;
        }
        if (tecla == ESPACIO) return 3;
        if (tecla == ENTER) return 4;
        return 0;
    }
    


#else
    #include <unistd.h>
    #include <termios.h>
    #define LIMPIAR_PANTALLA "clear"

    int leer_tecla(void) {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        ch = getchar();
        if (ch == 27) {
            getchar();
            switch (getchar()) {
                case 'A': ch = 1; break;
                case 'B': ch = 2; break;
                default: ch = 0; break;
            }
        } else if (ch == ' ') {
            ch = 3;
        } else if (ch == '\n' || ch == 13) {
            ch = 4;
        } else {
            ch = 0;
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }

#endif

typedef struct alumno {
    int legajo;
    char nombre[100];
    char apellido[100];
    float promedio;
} Alumno;

int sumar(int, int);
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
int leer_tecla(void);
int leer_archivo(void);
int main(void) {
    const char *opciones[TOTAL_OPCIONES] = {
        "Suma",
        "CHAR",
        "STRUCT",
        "PUNTEROS",
        "MALLOC",
        "MALLOC STRUCT",
        "LEER ARCHIVO",
        "Salir"
    };
    int seleccionados[TOTAL_OPCIONES];
    int cursor = 0;
    int continuar = 1;
    int i, accion, elegida;

    for (i = 0; i < TOTAL_OPCIONES; i++)
        seleccionados[i] = 0;

    while (continuar) {
        system(LIMPIAR_PANTALLA);
        printf("--- MENU (Flechas: mover | Espacio: marcar | Enter: confirmar) ---\n\n");

        for (i = 0; i < TOTAL_OPCIONES; i++) {
            printf("%s %s %s\n",
                (i == cursor) ? "->" : "  ",
                (seleccionados[i]) ? "[X]" : "[ ]",
                opciones[i]);
        }

        accion = leer_tecla();
        switch (accion) {
            case 1:
                cursor = (cursor == 0) ? TOTAL_OPCIONES - 1 : cursor - 1;
                break;
            case 2:
                cursor = (cursor == TOTAL_OPCIONES - 1) ? 0 : cursor + 1;
                break;
            case 3:
                /* una sola opcion a la vez (radio) */
                for (i = 0; i < TOTAL_OPCIONES; i++)
                    seleccionados[i] = 0;
                seleccionados[cursor] = 1;
                break;
            case 4:
                if(seleccionados[cursor] == 0) {
                    continuar = 1;
                }
                else {
                    continuar = 0;
                }
                break;
        }
    }

    elegida = -1;
    for (i = 0; i < TOTAL_OPCIONES; i++) {
        if (seleccionados[i]) {
            elegida = i;
            break;
        }
    }

    limpiar_pantalla();
    if (elegida < 0) {
        printf("No se selecciono ninguna opcion\n");
    } else if (elegida == 0) {
        clase_suma();
    } else if (elegida == 1) {
        clase_char();
    } else if (elegida == 2) {
        clase_struct();
    } else if (elegida == 3) {
        clase_punteros();
    } else if (elegida == 4) {
        clase_malloc();
    } else if (elegida == 5) {
        clase_malloc_struct();
    } else if (elegida == 6) {
        leer_archivo();
    } else {
        clase_salir();
    }
    /* elegida == 6: Salir */

    if (elegida != TOTAL_OPCIONES - 1)
        clase_salir();
    return 0;
}

int clase_suma(void) {
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

int sumar(int a, int b) {
    return a + b;
}

void clase_char(void) {
    char nombre[100];
    printf("Ingrese su nombre: ");
    while (getchar() != '\n')
        ;
    /* fgets(nombre, 100, stdin);*/
    scanf("%s", nombre);
    /* fputs(nombre, stdout);*/
    printf("\nEl nombre ingresado es: %s", nombre);
}

void clase_struct(void) {
    Alumno alumno1;
    printf("\n Ingrese el legajo del alumno: ");
    scanf("%d", &alumno1.legajo);
    while (getchar() != '\n')
        ;
    printf("\n Ingrese el nombre del alumno: ");
    /* fgets(alumno1.nombre, 100, stdin);*/
    scanf("%s", alumno1.nombre);
    printf("\n Ingrese el apellido del alumno: ");
    /* fgets(alumno1.apellido, 100, stdin);*/
    scanf("%s", alumno1.apellido);
    printf("\n Ingrese el promedio del alumno: ");
    scanf("%f", &alumno1.promedio);
    printf("\n El legajo del alumno es: %d\n", alumno1.legajo);
    printf("\n El nombre del alumno es: ");
    /* fputs(alumno1.nombre, stdout);*/
    printf("%s", alumno1.nombre);
    printf("\n El apellido del alumno es: ");
    /* fputs(alumno1.apellido, stdout);*/
    printf("%s", alumno1.apellido);
    printf("\n El promedio del alumno es: %.2f", alumno1.promedio);
}

void clase_punteros(void) {
    int *puntero;
    int valor = 10;
    puntero = &valor;
    printf("\n El valor del puntero es: %d", *puntero);
}

void clase_malloc(void) {
    int *puntero;
    int i, tam = 10;
    char opcion;
    puntero = (int *)malloc(sizeof(int) * tam);
    if (puntero == NULL) {
        printf("Error al asignar memoria\n");
        return;
    }
    for (i = 0; i < tam; i++) {
        printf("\n Ingrese el valor %d: ", i + 1);
        scanf("%d", &puntero[i]);
    }
    for (i = 0; i < tam; i++) {
        printf("\n El valor %d es: %d", i + 1, puntero[i]);
    }

    printf("\n Desea ordenar los valores? (s/n): ");
    while (getchar() != '\n')
        ;
    scanf("%c", &opcion);
    if (opcion == 's') {
        bubble_sort(puntero, tam);
        for (i = 0; i < tam; i++) {
            printf("\n El valor %d es: %d", i + 1, puntero[i]);
        }
    }

    free(puntero);
}

int clase_malloc_struct(void) {
    char nombre_archivo[100];
    Alumno *puntero;
    int i, tam;

    printf("Ingrese el numero de alumnos: ");
    scanf("%d", &tam);

    puntero = (Alumno *)malloc(sizeof(Alumno) * tam);
    if (puntero == NULL) {
        printf("Error al asignar memoria\n");
        return 0;
    }
    for (i = 0; i < tam; i++) {
        printf("Ingrese el legajo del alumno: ");
        scanf("%d", &puntero[i].legajo);
        printf("Ingrese el nombre del alumno: ");
        while (getchar() != '\n')
            ;
        /* fgets(puntero[i].nombre, 100, stdin); */
        scanf("%s", puntero[i].nombre);
        printf("Ingrese el apellido del alumno: ");
        /*fgets(puntero[i].apellido, 100, stdin);*/
        scanf("%s", puntero[i].apellido);
        printf("Ingrese el promedio del alumno: ");
        scanf("%f", &puntero[i].promedio);
        limpiar_pantalla();
    }
    for (i = 0; i < tam; i++) {
        printf("\n Alumno %d:", i + 1);
        printf("\n El legajo del alumno es: %d", puntero[i].legajo);
        printf("\n El nombre del alumno es: %s", puntero[i].nombre);
        printf("\n El apellido del alumno es: %s", puntero[i].apellido);
        printf("\n El promedio del alumno es: %.2f", puntero[i].promedio);
        printf("\n\n--------------------o--------------------\n");
    }

    printf("Ingrese el nombre del archivo: ");
    scanf("%s", nombre_archivo);
    guardar_datos_en_archivo(nombre_archivo, puntero, tam);

    free(puntero);
    return 0;
}

void clase_salir(void) {
    printf("\nPresione Enter para salir\n");
    while (getchar() != '\n')
        ;
    getchar();
}

int bubble_sort(int *puntero, int tam) {
    int i, j, aux;
    for (i = 0; i < tam - 1; i++) {
        for (j = 0; j < tam - i - 1; j++) {
            if (puntero[j] > puntero[j + 1]) {
                aux = puntero[j];
                puntero[j] = puntero[j + 1];
                puntero[j + 1] = aux;
            }
        }
    }
    return 0;
}

void limpiar_pantalla(void) {
    system(LIMPIAR_PANTALLA);
}

int guardar_datos_en_archivo(char *nombre_archivo, Alumno *puntero, int tam) {
    int i;
    FILE *archivo;
    if(strchr(nombre_archivo, '.') == NULL) {
        strcat(nombre_archivo, ".dat");
    }
    /**/
    if (access(nombre_archivo, F_OK) == 0) {
        archivo = fopen(nombre_archivo, "ab");
    }
    else{
        archivo = fopen(nombre_archivo, "wb");
    }

    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return 0;
    }
    for (i = 0; i < tam; i++) {
        fwrite(&puntero[i], sizeof(puntero[i]), 1, archivo);
    }

    fclose(archivo);
    return 0;
}


int leer_archivo(void) {
    char nombre_archivo[100];
    Alumno puntero;
    FILE *archivo;
    printf("Ingrese el nombre del archivo: ");
    scanf("%s", nombre_archivo);
    if(strchr(nombre_archivo, '.') == NULL) {
        strcat(nombre_archivo, ".dat");
    }
    archivo = fopen(nombre_archivo, "rb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return 0;
    }
    while (fread(&puntero, sizeof(puntero), 1, archivo) != 0) {
        printf("\n El legajo del alumno es: %d", puntero.legajo);
        printf("\n El nombre del alumno es: %s", puntero.nombre);
        printf("\n El apellido del alumno es: %s", puntero.apellido);
        printf("\n El promedio del alumno es: %.2f", puntero.promedio);
        printf("\n\n--------------------o--------------------\n");
    }
    fclose(archivo);
    return 0;
}

/*
int leer_archivo(void) {
    char nombre_archivo[100];
    char linea[100];
    FILE *archivo;
    printf("Ingrese el nombre del archivo: ");
    scanf("%s", nombre_archivo);
    archivo = fopen(nombre_archivo, "rb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return 0;
    }
    while (fgets(linea, 100, archivo) != NULL) {
        printf("%s", linea);
    }
    fclose(archivo);
    return 0;
}
*/

/* 
fseek(archivo, 0, SEEK_SET); 
establece el puntero de lectura/escritura al inicio del archivo
fseek(archivo, 0, SEEK_END);
establece el puntero de lectura/escritura al final del archivo
fseek(archivo, 0, SEEK_CUR);
establece el puntero de lectura/escritura en la posición actual
*/


