#include <stdio.h>

void echange( int *px , int *py){
    int z;
    printf("x = %d y = %d\n", *px, *py);
    z = *px;
    *px = *py;
    *py = z;
    printf("x = %d y = %d\n", *px, *py);
}

int main() {
    int a = 10, b = 20;
    printf("a = %d b = %d\n", a, b);
    
    echange( &a, &b);
    
    printf("a = %d b = %d\n", a, b);
    
    return 0;
}
