#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_OPCIONES 8

#define MAX_RUTA 512
#define MAX_NOMBRE_FS 256
#define MAX_ENTRADAS 256
#define MODO_GUARDAR 0
#define MODO_ABRIR 1

#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h>
    #include <io.h>
    #include <direct.h>
    #define LIMPIAR_PANTALLA "cls"
    #define SEP_RUTA '\\'
    #define ESPACIO 32
    #define ENTER 13
    #define access _access
    #ifndef F_OK
    #define F_OK 0
    #endif
    #define fs_chdir _chdir
    #define fs_getcwd _getcwd

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

    int listar_entradas(char nombres[][MAX_NOMBRE_FS], int *es_dir, int max) {
        struct _finddata_t info;
        long handle;
        int n = 0;

        handle = _findfirst("*.*", &info);
        if (handle == -1L)
            return 0;
        do {
            if (strcmp(info.name, ".") == 0)
                continue;
            if (n >= max)
                break;
            strncpy(nombres[n], info.name, MAX_NOMBRE_FS - 1);
            nombres[n][MAX_NOMBRE_FS - 1] = '\0';
            es_dir[n] = (info.attrib & _A_SUBDIR) ? 1 : 0;
            n++;
        } while (_findnext(handle, &info) == 0);
        _findclose(handle);
        return n;
    }

#else
    #include <unistd.h>
    #include <termios.h>
    #include <dirent.h>
    #include <sys/stat.h>
    #define LIMPIAR_PANTALLA "clear"
    #define SEP_RUTA '/'
    #define fs_chdir chdir
    #define fs_getcwd getcwd

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

    int listar_entradas(char nombres[][MAX_NOMBRE_FS], int *es_dir, int max) {
        DIR *dir;
        struct dirent *ent;
        struct stat st;
        int n = 0;

        dir = opendir(".");
        if (dir == NULL)
            return 0;
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0)
                continue;
            if (n >= max)
                break;
            if (stat(ent->d_name, &st) != 0)
                continue;
            strncpy(nombres[n], ent->d_name, MAX_NOMBRE_FS - 1);
            nombres[n][MAX_NOMBRE_FS - 1] = '\0';
            es_dir[n] = S_ISDIR(st.st_mode) ? 1 : 0;
            n++;
        }
        closedir(dir);
        return n;
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
int listar_entradas(char [][MAX_NOMBRE_FS], int *, int);
void ordenar_entradas(char [][MAX_NOMBRE_FS], int *, int);
int navegar_fs(int, char *, int);
int elegir_ruta_guardar(char *, int);
int elegir_ruta_abrir(char *, int);
void unir_ruta(char *, int, const char *, const char *);

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
        exit(1);
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
    char nombre_archivo[MAX_RUTA];
    Alumno *puntero;
    int i, tam;

    printf("Ingrese el numero de alumnos: ");
    scanf("%d", &tam);

    puntero = (Alumno *)malloc(sizeof(Alumno) * tam);
    if (puntero == NULL) {
        printf("Error al asignar memoria\n");
        exit(1);
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

    if (!elegir_ruta_guardar(nombre_archivo, (int)sizeof(nombre_archivo))) {
        printf("No se eligio ruta de guardado\n");
        free(puntero);
        return 0;
    }
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

void unir_ruta(char *destino, int tam, const char *dir, const char *nombre) {
    int len_dir, len_nombre, necesita_sep;

    len_dir = (int)strlen(dir);
    len_nombre = (int)strlen(nombre);
    necesita_sep = (len_dir > 0 && dir[len_dir - 1] != SEP_RUTA) ? 1 : 0;
    if (len_dir + necesita_sep + len_nombre + 1 > tam) {
        destino[0] = '\0';
        return;
    }
    strcpy(destino, dir);
    if (necesita_sep) {
        destino[len_dir] = SEP_RUTA;
        destino[len_dir + 1] = '\0';
    }
    strcat(destino, nombre);
}

void ordenar_entradas(char nombres[][MAX_NOMBRE_FS], int *es_dir, int n) {
    int i, j;
    char tmp[MAX_NOMBRE_FS];
    int tmp_dir;
    /* ponytail: O(n^2) bubble; ok hasta MAX_ENTRADAS; upgrade: qsort */
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (es_dir[j] < es_dir[j + 1] ||
                (es_dir[j] == es_dir[j + 1] && strcmp(nombres[j], nombres[j + 1]) > 0)) {
                strcpy(tmp, nombres[j]);
                strcpy(nombres[j], nombres[j + 1]);
                strcpy(nombres[j + 1], tmp);
                tmp_dir = es_dir[j];
                es_dir[j] = es_dir[j + 1];
                es_dir[j + 1] = tmp_dir;
            }
        }
    }
}

int navegar_fs(int modo, char *ruta, int tam_ruta) {
    char nombres[MAX_ENTRADAS][MAX_NOMBRE_FS];
    int es_dir[MAX_ENTRADAS];
    int map[MAX_ENTRADAS];
    char actual[MAX_RUTA];
    int n, visible, cursor, accion, i, total_menu, sel;

    cursor = 0;
    for (;;) {
        if (fs_getcwd(actual, MAX_RUTA) == NULL) {
            printf("No se pudo obtener el directorio actual\n");
            exit(1);
        }

        n = listar_entradas(nombres, es_dir, MAX_ENTRADAS);
        ordenar_entradas(nombres, es_dir, n);

        visible = 0;
        for (i = 0; i < n; i++) {
            if (modo == MODO_GUARDAR && !es_dir[i])
                continue;
            map[visible] = i;
            visible++;
        }

        total_menu = visible + 1; /* + Cancelar */
        if (modo == MODO_GUARDAR)
            total_menu++; /* + Confirmar carpeta */

        if (cursor >= total_menu)
            cursor = total_menu - 1;
        if (cursor < 0)
            cursor = 0;

        limpiar_pantalla();
        printf("Directorio: %s\n", actual);
        if (modo == MODO_GUARDAR)
            printf("Flechas | Enter: entrar | Espacio o Confirmar: usar carpeta\n\n");
        else
            printf("Flechas | Enter/Espacio: entrar carpeta o elegir archivo\n\n");

        for (i = 0; i < visible; i++) {
            sel = map[i];
            printf("%s %s%s\n",
                (i == cursor) ? "->" : "  ",
                nombres[sel],
                es_dir[sel] ? "/" : "");
        }
        if (modo == MODO_GUARDAR) {
            printf("%s [ Confirmar esta carpeta ]\n",
                (cursor == visible) ? "->" : "  ");
            printf("%s [ Cancelar ]\n",
                (cursor == visible + 1) ? "->" : "  ");
        } else {
            printf("%s [ Cancelar ]\n",
                (cursor == visible) ? "->" : "  ");
        }

        accion = leer_tecla();
        if (accion == 1) {
            cursor = (cursor == 0) ? total_menu - 1 : cursor - 1;
        } else if (accion == 2) {
            cursor = (cursor == total_menu - 1) ? 0 : cursor + 1;
        } else if (accion == 3 || accion == 4) {
            if (modo == MODO_GUARDAR) {
                if (accion == 3 || cursor == visible) {
                    strncpy(ruta, actual, tam_ruta - 1);
                    ruta[tam_ruta - 1] = '\0';
                    return 1;
                }
                if (cursor == visible + 1)
                    return 0;
                sel = map[cursor];
                if (fs_chdir(nombres[sel]) != 0)
                    printf("No se pudo entrar a %s\n", nombres[sel]);
                cursor = 0;
            } else {
                if (cursor == visible)
                    return 0;
                sel = map[cursor];
                if (es_dir[sel]) {
                    if (fs_chdir(nombres[sel]) != 0)
                        printf("No se pudo entrar a %s\n", nombres[sel]);
                    cursor = 0;
                } else {
                    unir_ruta(ruta, tam_ruta, actual, nombres[sel]);
                    return 1;
                }
            }
        }
    }
}

int elegir_ruta_guardar(char *ruta, int tam) {
    char dir[MAX_RUTA];
    char nombre[100];

    if (!navegar_fs(MODO_GUARDAR, dir, MAX_RUTA))
        return 0;

    limpiar_pantalla();
    printf("Carpeta: %s\n", dir);
    printf("Ingrese el nombre del archivo: ");
    scanf("%99s", nombre);
    if (strchr(nombre, '.') == NULL)
        strcat(nombre, ".dat");
    unir_ruta(ruta, tam, dir, nombre);
    return 1;
}

int elegir_ruta_abrir(char *ruta, int tam) {
    return navegar_fs(MODO_ABRIR, ruta, tam);
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
        exit(1);
    }
    for (i = 0; i < tam; i++) {
        fwrite(&puntero[i], sizeof(puntero[i]), 1, archivo);
    }
    fclose(archivo);
    printf("Path del archivo: %s\n", nombre_archivo);
    printf("\n\n--------------------o--------------------\n");
    return 0;
}


int leer_archivo(void) {
    char nombre_archivo[MAX_RUTA];
    Alumno puntero;
    FILE *archivo;

    if (!elegir_ruta_abrir(nombre_archivo, MAX_RUTA)) {
        printf("No se eligio archivo\n");
        exit(1);
    }
    limpiar_pantalla();

    archivo = fopen(nombre_archivo, "rb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        exit(1);
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
fseek(archivo, 0, SEEK_SET); 
establece el puntero de lectura/escritura al inicio del archivo
fseek(archivo, 0, SEEK_END);
establece el puntero de lectura/escritura al final del archivo
fseek(archivo, 0, SEEK_CUR);
establece el puntero de lectura/escritura en la posición actual
*/