#include <stdio.h>
int sumar(int , int);


int main(){
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
    printf("Presione enter para salir\n");
    
    return 0;
}

int sumar(int a, int b){
    return a + b;
}