#include <stdio.h>
int sumar(int , int);


int main(){
    int val1, val2, resultado;
    printf("Ingrese el primer valor: ");
    scanf("%d", &val1);
    if (val1 < 0) {
        printf("El valor ingresado es invalido\n");
        return 0;
    }
    else if (val1 == 0) {
        printf("El valor ingresado es 0\n");
        return 0;
    }
    printf("Ingrese el segundo valor: ");
    scanf("%d", &val2);
    if (val2 < 0) {
        printf("El valor ingresado es invalido\n");
        return 0;
    }
    else if (val2 == 0) {
        printf("El valor ingresado es 0\n");
        return 0;
    }
    resultado = sumar(val1, val2);
    printf("El resultado de la suma es: %d\n", resultado);
    return 0;
}

int sumar(int a, int b){
    return a + b;
}