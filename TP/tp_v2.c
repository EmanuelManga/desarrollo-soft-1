#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


#else
    #include <unistd.h>
    #include <termios.h>
    #include <dirent.h>
    #include <sys/stat.h>
    #define LIMPIAR_PANTALLA "clear"
    #define SEP_RUTA '/'
    #define fs_chdir chdir
    #define fs_getcwd getcwd

#endif

#define NOMBRE_ARCHIVO "productos.dat"
#define NOMBRE_ARCHIVO_ORIGINAL "productos.dat.original"
#define DISCONTINUADO "Producto Discontinuo"
#define DISCONTINUADO_CANTIDAD 10

typedef struct producto {
    int codigo;
    char marca[30];
    int cantidad;
} Producto;


void menu(void);
void limpiar_pantalla(void);
void cargar_producto(void);
void listar_archivo(void);
void listar_sin_formato(void);
void salir(void);
void cargar_archivo(char *nombre_archivo, Producto *producto, int tam);
int leer_productos(char *nombre_archivo, Producto **productos);
void volver_menu(void);
int buscar_producto_indice(Producto *productos, int tam, int codigo);

int main(void) {
    /* Funcion principal */
    menu();
    
    /* volver_menu(); */

    printf("\n\nPresione Enter para salir\n");
    while(getchar() != '\n');
    getchar();
    return 0;

    
}

void volver_menu(void) {
    char opcion;
    printf("\nVolver al menu? (s/n): ");
    while(getchar() != '\n');
    scanf("%c", &opcion);
    if (opcion == 's') {
        menu();
    }
}

void menu(void) {
    int opcion, opcion_valida = 0;
    while (opcion_valida != 1) {
        limpiar_pantalla();
        printf("Deposito de zapatillas\n");
        printf("1. Cargar archivo\n");
        printf("2. Listar archivo\n");
        printf("3. Listar sin formatear\n");
        printf("4. Salir\n");
        printf("Ingrese una opcion: ");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                opcion_valida = 1;
                limpiar_pantalla();
                cargar_producto();
                break;
            case 2:
                opcion_valida = 1;
                limpiar_pantalla();
                listar_archivo();
                break;
            case 3:
                opcion_valida = 1;
                limpiar_pantalla();
                listar_sin_formato();
                break;
            case 4:
                opcion_valida = 1;
                break;
            default:
                printf("Opcion no valida\n");
                break;
        }
    }
}

void limpiar_pantalla(void) {
    system(LIMPIAR_PANTALLA);
}


void cargar_producto(void) {
    Producto *productos;
    Producto actual;
    Producto *aux;
    int n;
    int codigo_existente;
    int cambio;

    n = leer_productos(NOMBRE_ARCHIVO, &productos);
    cambio = 0;

    printf("Ingrese el codigo del producto: ");
    scanf("%d", &actual.codigo);
    while (actual.codigo != 0) {
        /*Si el producto ya existe, se actualiza la
        cantidad y no pide la marca*/
        codigo_existente = buscar_producto_indice(productos, n, actual.codigo);
        if (codigo_existente != -1) {
            printf("Ingrese la cantidad del producto: ");
            scanf("%d", &actual.cantidad);
            productos[codigo_existente].cantidad += actual.cantidad;
            cambio = 1;
        }
        else{
            printf("Ingrese la marca del producto: ");
            scanf("%s", actual.marca);
            printf("Ingrese la cantidad del producto: ");
            scanf("%d", &actual.cantidad);
            aux = (Producto *)realloc(productos, (n + 1) * sizeof(Producto));
            if (aux == NULL) {
                printf("Error al asignar memoria\n");
                free(productos);
                return;
            }
            productos = aux;
            productos[n] = actual;
            n++;
            cambio = 1;
        }

        limpiar_pantalla();

        printf("Ingrese el codigo del producto: ");
        scanf("%d", &actual.codigo);
    }

    if (cambio)
        cargar_archivo(NOMBRE_ARCHIVO, productos, n);

    free(productos);
    printf("Archivo cargado correctamente\n");
    printf("\n\n--------------------o--------------------\n");
    volver_menu();
}

int leer_productos(char *nombre_archivo, Producto **productos) {
    FILE *archivo;
    long bytes;
    int n;

    *productos = NULL;
    archivo = fopen(nombre_archivo, "rb");
    if (archivo == NULL)
        return 0;

    if (fseek(archivo, 0, SEEK_END) != 0) {
        printf("Error al leer el archivo\n");
        fclose(archivo);
        exit(1);
    }
    bytes = ftell(archivo);
    if (bytes < 0 || fseek(archivo, 0, SEEK_SET) != 0) {
        printf("Error al leer el archivo\n");
        fclose(archivo);
        exit(1);
    }

    n = (int)(bytes / (long)sizeof(Producto));
    if (n > 0) {
        *productos = (Producto *)malloc(n * sizeof(Producto));
        if (*productos == NULL) {
            printf("Error al asignar memoria\n");
            fclose(archivo);
            exit(1);
        }
        if (fread(*productos, sizeof(Producto), n, archivo) != (size_t)n) {
            printf("Error al leer el archivo\n");
            free(*productos);
            *productos = NULL;
            fclose(archivo);
            exit(1);
        }
    }
    fclose(archivo);
    return n;
}

int buscar_producto_indice( Producto *productos, int tam, int codigo){
    int i;
    for (i = 0; i < tam; i++) {
        if (productos[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

void listar_archivo(void) {
    FILE *archivo;
    Producto producto;

    archivo = fopen(NOMBRE_ARCHIVO, "rb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        exit(1);
    }
    while(fread(&producto, sizeof(Producto), 1, archivo) == 1) {
        printf("Codigo: %d\n", producto.codigo);
        if (producto.cantidad < DISCONTINUADO_CANTIDAD) {
            printf("Marca: %s\n", DISCONTINUADO);

        }
        else{
            printf("Marca: %s\n", producto.marca);

        }
        printf("Cantidad: %d\n", producto.cantidad);
        printf("\n--------------------o--------------------\n\n");
    }
    fclose(archivo);
    printf("Listado completo\n");
    printf("\n\n--------------------o--------------------\n");
    volver_menu();
}

void listar_sin_formato(void) {
    FILE *archivo;
    Producto producto;

    archivo = fopen(NOMBRE_ARCHIVO, "rb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        exit(1);
    }
    while(fread(&producto, sizeof(Producto), 1, archivo) == 1) {
        printf("%d %s %d\n", producto.codigo, producto.marca, producto.cantidad);
    }
    fclose(archivo);
    volver_menu();
}

void salir(void) {
    printf("Saliendo...\n");
}


void cargar_archivo(char *nombre_archivo, Producto *producto, int tam) {
    FILE *archivo;

    /* El arreglo ya trae el archivo entero, se reescribe */
    archivo = fopen(nombre_archivo, "wb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        exit(1);
    }
    fwrite(producto, sizeof(Producto), tam, archivo);
    fclose(archivo);
}
