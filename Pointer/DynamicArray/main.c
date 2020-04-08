#include <stdio.h>
#include <stdlib.h>

int Sum( int *Array, int SizeArray){
    int Sum = 0;
    for (int index = 0; index < SizeArray; ++index) {
        Sum += Array[index];
    }
    return Sum;
}

void SetArrayToZero( int *Array, int SizeArray ){
    //if there is only one line in the for loop, the {} are not needed
    for (int index = 0; index < SizeArray; ++index) Array[index] = 0;
}

int main() {
    
    unsigned int SizeX, SizeY;
    printf("Please enter the size (positive integer) of array Array_X and then for Array_Y\nTry to enter also 0 to see what happens\n");
    if ( scanf( "%u %u", &SizeX, &SizeY) != 2) {
        printf("You did not give me two unsigned int\nI will terminate now\n");
        return 0;
    }
    
    int* Array_X = (int*) malloc( SizeX * sizeof(int) ); // no initialisation (values are arbitrary)
    // Check if the memory has been successfully
    // allocated by malloc or not
    if (Array_X == NULL) {
        printf("Array_X : Memory not allocated.\nI will terminate now\n");
        exit(0);
    }
    
    int* Array_Y = (int*) calloc( SizeY, sizeof(int) ); // all values are initialized to 0
    if (Array_Y == NULL) {
        printf("Array_Y : Memory not allocated.\nI will terminate now\n");
        exit(0);
    }
    
    //All values of Array_X are arbitrary until now
    //Fill the COMPLETE Array_X
    for (unsigned int index = 0; index < SizeX; ++index) Array_X[index] = index;
    //if there is only one line in the for loop, the {} are not needed
    
    //All values of Array_Y are 0, I want ot change only two valus
    Array_Y[0] = 3;//first value
    Array_Y[SizeY-1] = 2;//last value
    
    printf("Sum of all values in Array_X = %d\n", Sum( Array_X, SizeX));
    printf("Sum of all values in Array_Y = %d\n", Sum( Array_Y, SizeY));
    
    printf("\nCall function SetArrayToZero( Array_X, SizeX )\n");
    SetArrayToZero( Array_X, SizeX );
    printf("Sum of all values in Array_X = %d\n", Sum( Array_X, SizeX));
    
    //free memory
    free(Array_X);
    free(Array_Y);
    
    return 0;
}
