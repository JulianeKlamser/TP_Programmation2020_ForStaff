#include <stdio.h>
#define SizeX 10
#define SizeY 20

int Sum( int *Array, int SizeArray){
    int Sum = 0;
    for (int index = 0; index < SizeArray; ++index) {
        Sum += Array[index];
    }
    return Sum;
}


int main() {
    
    int Array_X[SizeX], Array_Y[SizeY];
    //Fill Array_X
    for (int index = 0; index < SizeX; ++index) Array_X[index] = index;
    //if there is only one line in the for loop, the {} are not needed
    
    //Fill Array_Y
    for (int index = 0; index < SizeY; ++index) Array_Y[index] = index*index;
    
    printf("Sum of all values in Array_X = %d\n", Sum( Array_X, SizeX));
    printf("Sum of all values in Array_Y = %d\n", Sum( Array_Y, SizeY));
    
    return 0;
}
