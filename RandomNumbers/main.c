#include <stdio.h>
#include <stdlib.h> //for random numbers
#include <time.h> // for wall time
#include <string.h>  // to change an character array
#define FileNameSize 100

unsigned * DynamicallyAllocate1DUnsigedArray( unsigned ArraySize ){
    unsigned * Array;
    Array = calloc(ArraySize , sizeof(unsigned));
    if( Array == NULL ){
        printf("Memory allocation of unsigned array failed\n");
        exit(0);
    }
    return Array;
}

double * DynamicallyAllocate1DDoubleArray( unsigned ArraySize ){
    double * Array;
    Array = calloc(ArraySize , sizeof(double));
    if( Array == NULL ){
        printf("Memory allocation of double array failed\n");
        exit(0);
    }
    return Array;
}

unsigned ReadUnsigendIntegerFromKeyboard (void){
    unsigned Size = 0;
    if ( scanf("%u", &Size) != 1) {
        printf("Failed to read spiral size. Need positive integer number\n");
        exit(0);
    }
    return Size;
}

unsigned Write1DArrayOfUnsigneds( unsigned * Array, unsigned ArraySize , char Filename[FileNameSize]){
    unsigned Test = 1;
    FILE *PointerToFile;
    PointerToFile = fopen(Filename, "w"); // open(/create) file Filename
    if (PointerToFile == NULL) Test = 0;
    else{
        for (unsigned i = 0; i < ArraySize; ++i) {
            fprintf(PointerToFile, "%u %u\n", i, Array[i]);//writing data into file.
        }
        fclose(PointerToFile);//close File
    }
    return Test;
}

unsigned Write1DArrayOfDouble( double * Array, unsigned ArraySize , char Filename[FileNameSize]){
    unsigned Test = 1;
    FILE *PointerToFile;
    PointerToFile = fopen(Filename, "w"); // open(/create) file Filename
    if (PointerToFile == NULL) Test = 0;
    else{
        for (unsigned i = 0; i < ArraySize; ++i) {
            fprintf(PointerToFile, "%u %f\n", i, Array[i]);//writing data into file.
        }
        fclose(PointerToFile);//close File
    }
    return Test;
}


int main(void) {
    char Filename[FileNameSize];
    unsigned Test;
    unsigned Ntotal;
    
    //START **************** Seed pseudo random number generator ******************************
    printf("Press 1 if you want to generate each time the same set of random numbers.\nPress 2 if you want to generate each time a different set of random numbers.\nPlease enter your choice: ");
    Test = ReadUnsigendIntegerFromKeyboard ();
    if (Test == 1) {
        srand(1); // seed of pseudo random number generator is 1
        printf("Your choice: Same series of pseudo random numbers\n");
    }
    else{
        srand((unsigned) time(0));  // seed of pseudo random number generator is the wall time
        printf("\nYour choice: Different series of pseudo random numbers\n\n");
    }
    //ENS **************** Seed pseudo random number generator ******************************
    
    //START ++++++++++++++ random integers in the intervall [0,50) ++++++++++++++
    printf("How many random integers in the intervall [0,50) do you want to generate?\nChoose a positive integer number.\nPlease enter your choice: ");
    Ntotal = ReadUnsigendIntegerFromKeyboard ();
    unsigned * RandomUns = DynamicallyAllocate1DUnsigedArray( Ntotal );
    for (unsigned run = 0; run < Ntotal; ++run) {
        RandomUns[ run ] = rand() % 50; //a random integer in intervall [0,50)
    }
    //--> Save RandomUns in file
    strcpy(Filename, "./RandomInt0To50.dat"); //file RandomInt0To50.dat is(/will be) in same directory as code
    Test = Write1DArrayOfUnsigneds( RandomUns, Ntotal, Filename);
    if (Test == 0) printf("\n-----> Failed to open %s .\n\n", Filename);
    else printf("\n-----> File %s was successfully saved.\n\n", Filename);
    free(RandomUns);//<-------- FREE : DO Not forget me ;)
    //END ++++++++++++++ random integers in the intervall [0,50) ++++++++++++++
    
    
    //START ------------ random doubles in the intervall (-2.5,2.5) ------------
    printf("How many random doubles in the intervall (-2.5,2.5) do you want to generate?\nChoose a positive integer number.\nPlease enter your choice: ");
    Ntotal = ReadUnsigendIntegerFromKeyboard ();
    double * RandomDoubles = DynamicallyAllocate1DDoubleArray( Ntotal );
    for (unsigned run = 0; run < Ntotal; ++run) {
        double RandDouble = (double) rand() / RAND_MAX; //Random double in intervall (0.0,1.0)
        RandomDoubles[ run ] = 5.0 * RandDouble - 2.5;
    }
    //--> Save RandomDoubles in file
    strcpy(Filename, "./RandomDoubleList.dat"); //file RandomDoubleList.dat is(/will be) in same directory as code
    Test = Write1DArrayOfDouble( RandomDoubles, Ntotal , Filename);
    if (Test == 0) printf("\n-----> Failed to open %s .\n\n", Filename);
    else printf("\n-----> File %s was successfully saved.\n\n", Filename);
    free(RandomDoubles);//<-------- FREE : DO Not forget me ;)
    //END ------------ random doubles in the intervall (-2.5,2.5) ------------
    
    return 0;
}
