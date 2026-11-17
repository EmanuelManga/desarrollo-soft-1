#include <stdio.h>


int fibo(int a, int b){
    if (a <= 0) {
        return 1;
    }
    else {
        return a + b;
    }
}

int fibonachi_mio(void){
    int n,i,resultado,anterior;
    printf("\n Ingresar N:");
    scanf("%d",&n);
    for (i = 0; i < n; i++) {
        resultado = fibo(anterior,resultado);
        anterior = resultado - anterior;
        printf("\n%d %d",i,anterior);
    }

    return 0;

}

int fibonachi_ia(void){
    int n,i,resultado,anterior;
    printf("\n Ingresar N:");
    scanf("%d",&n);
    for (i = 0; i < n; i++) {
        resultado = fibo(anterior,resultado);
        anterior = resultado - anterior;
        printf("\n%d %d",i+1,anterior);
    }
    return 0;
}


int main(void){
    fibonachi_ia();
    return 0;
}