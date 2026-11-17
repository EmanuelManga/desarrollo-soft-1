#include <stdio.h>
#include <stdlib.h>

#define TOTAL_OPCIONES 3

#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h>
    #define LIMPIAR_PANTALLA "cls"
    #define ARRIBA 72
    #define ABAJO 80
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
            switch(getchar()) {
                case 'A': ch = 1; break; 
                case 'B': ch = 2; break; 
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


int main(void) {
    const char *opciones[TOTAL_OPCIONES] = {"Opcion A", "Opcion B", "Opcion C"};
    int seleccionados[TOTAL_OPCIONES] = {0, 0, 0};
    int cursor = 0;
    int continuar = 1;
    int i,accion;

    while (continuar) {
        system(LIMPIAR_PANTALLA);
        printf("--- MENU MULTIPLATAFORMA (Espacio: Marcar | Enter: Confirmar) ---\n\n");

        for ( i = 0; i < TOTAL_OPCIONES; i++) {
            printf("%s %s %s\n", 
                (i == cursor) ? "->" : "  ", 
                (seleccionados[i]) ? "[X]" : "[ ]", 
                opciones[i]);
        }

        accion = leer_tecla();
        switch(accion) {
            case 1: 
                cursor = (cursor == 0) ? TOTAL_OPCIONES - 1 : cursor - 1;
                break;
            case 2: 
                cursor = (cursor == TOTAL_OPCIONES - 1) ? 0 : cursor + 1;
                break;
            case 3: 
                seleccionados[cursor] = !seleccionados[cursor];
                break;
            case 4: 
                continuar = 0;
                break;
        }
    }

    system(LIMPIAR_PANTALLA);
    printf("Opciones seleccionadas:\n");
    for ( i = 0; i < TOTAL_OPCIONES; i++) {
        if (seleccionados[i]) printf("- %s\n", opciones[i]);
    }

    return 0;
}
