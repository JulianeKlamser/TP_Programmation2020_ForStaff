#include <stdio.h>
#include <stdlib.h>

void Init2DSquareArray( int **Array, unsigned SizeArray, int InitValue ){
    //if there is only one line in the for loop, the {} are not needed
    for (unsigned xi = 0; xi < SizeArray; ++xi) {
        for (unsigned yi = 0; yi < SizeArray; ++yi) {
            Array[xi][yi] = InitValue;
        }
    }
}

void Print2DSquareArray( int **Array, unsigned SizeArray ){
    //if there is only one line in the for loop, the {} are not needed
    printf("Start Array <-----\n" );
    for (int yi = SizeArray-1; yi >= 0; --yi) {
        for (unsigned xi = 0; xi < SizeArray; ++xi) {
            printf("%d\t" , Array[xi][yi]);
        }
        printf("\n");
    }
    printf("End Array <-----\n\n" );
}

unsigned ReadOddSquareArraySize (void){
    printf("Choose an odd positive integer number\n Thise will be the size of your spiral.\n");
    unsigned Size = 0;
    if ( scanf("%u", &Size) != 1) {
        printf("Failed to read spiral size. Need positive integer number\n");
        exit(0);
    }
    if ( Size%2 == 0) {
        printf("You did not enter an odd integer number.\nTry again.\n");
        exit(0);
    }
    return Size;
}

void FillUlamSpiral( int **Array, unsigned SizeArray ){
    unsigned xi = SizeArray/2;
    unsigned yi = SizeArray/2;
    unsigned runner = 1;
    unsigned Limit = 1;
    int increment = +1;
    
    Array[xi][yi] = runner;
    while ( Limit < SizeArray) {
        for (unsigned helper = 0; helper < Limit; ++helper) {
            xi += increment;
            Array[xi][yi] = ++runner;
        }
        for (unsigned helper = 0; helper < Limit; ++helper) {
            yi += increment;
            Array[xi][yi] = ++runner;
        }
        increment *= -1;
        ++Limit;
    }
    //last row
    for (unsigned helper = 0; helper < Limit-1; ++helper) {
        xi += increment;
        Array[xi][yi] = ++runner;
    }
}


int ** DynamicallyAllocateInt2DSquareArray( unsigned Size ){
    int ** Array;//this variable will be destroyed after leaving the function
    //malloc will survive the function
    Array = (int**) malloc( Size * sizeof(int *));//only malloc possible!!!
    if( Array == NULL ){
        printf("-1 Memory allocation failed\n");
        exit(0);
    }
    for( unsigned xIndex = 0; xIndex < Size; xIndex++){
        //here malloc or calloc are possible
        Array[xIndex] = (int*) calloc(Size , sizeof(int));
        //Array[xIndex] = (int*) malloc(Size * sizeof(int));
        if(Array[xIndex] == NULL){
            printf("%d Memory allocation failed\n", xIndex);
            exit(0);
        }
    }
    return Array;
}

void Free2DSquareArray(int **Array, unsigned Size){
    for (unsigned xi = 0; xi < Size; xi++) free(Array[xi]);
    free(Array);
}

int main() {
        
    unsigned SpSize = ReadOddSquareArraySize();
    int ** Spiral;
    Spiral = DynamicallyAllocateInt2DSquareArray( SpSize );
    Print2DSquareArray( Spiral, SpSize );
    
    Init2DSquareArray( Spiral, SpSize, 5 );
    Print2DSquareArray( Spiral, SpSize );
    
    FillUlamSpiral( Spiral, SpSize );
    Print2DSquareArray( Spiral, SpSize );
    
    Free2DSquareArray(Spiral , SpSize);
    
    return 0;
}
