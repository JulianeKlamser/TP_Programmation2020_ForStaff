#include <stdio.h> //printf
#include <stdlib.h> //malloc
#define XLen_1 5
#define XLen_2 2
#define YLen 3

typedef struct{
    double X;
    double V;
}Particle;


//Static 1D array ----->------>----->------>----->------>----->------>
void initDouble1DArray( double Doub1DArray[], int ArrayLen){
    for (int i = 0; i < ArrayLen; ++i) {
        Doub1DArray[i] = i+0.5;
    }
}
void PrintDouble1DArray( double Doub1DArray[], int ArrayLen){
    for (int i = 0; i < ArrayLen; ++i) {
        printf("Double1DArray[%d] = %f\n", i, Doub1DArray[i]);
    }
}
//Static 1D array END END END END END END END END END END END END


//Static 2D array ----->------>----->------>----->------>----->------>
void initInt2DArray( int Int2DArray[][YLen], int ALenX){
    //printf("This initInt2DArray function for a static 2D array works only for y dimension YLen = %d\nsee #define at beginning\n", YLen);
    for (int xi = 0; xi < ALenX; ++xi) {
        for (int yi = 0; yi < YLen; ++yi) {
            Int2DArray[xi][yi] = xi + yi;
        }
    }
}
void PrintInt2DArray( int Int2DArray[][YLen], int ALenX){
    printf("This function for a static 2D array works only for y dimension YLen = %d\nsee #define at beginning\n", YLen);
    for (int xi = 0; xi < ALenX; ++xi) {
        for (int yi = 0; yi < YLen; ++yi) {
             printf("Int2DArray[%d][%d] = %d\t", xi, yi, Int2DArray[xi][yi]);
        }
        printf("\n");
    }
}
//Static 2D array END END END END END END END END END END END END


//Dynamic 1D array ----->------>----->------>----->------>----->------>
void init1DIntDynArray(  int * IntArray, int SizeIntArray){
    for (int i = 0; i < SizeIntArray; ++i) {
        IntArray[i] = i+1;
    }
}
void Print1DIntDynArray(  int * IntArray, int SizeIntArray){
    for (int i = 0; i < SizeIntArray; ++i) {
        printf("IntArray[%d] = %d\n", i, IntArray[i]);
    }
}
//Dynamic 1D array END END END END END END END END END END END END

//Dynamic 1D array New Type ----->------>----->------>----->------>----->------>
void init1DParticleDynArray( Particle * Particle1DArray, int Size){
    for (int i = 0; i < Size; ++i) {
        Particle1DArray[i].X = i+1;
        Particle1DArray[i].V = i+2;
    }
}
void Print1DParticleDynArray( Particle * Particle1DArray, int Size){
    for (int i = 0; i < Size; ++i) {
        printf("Particle1DArray[%d].X = %f\t", i, Particle1DArray[i].X);
        printf("Particle1DArray[%d].V = %f\n", i, Particle1DArray[i].V);
    }
}
//Dynamic 1D array New Type END END END END END END END END END END END END


//Dynamic 2D array ----->------>----->------>
void init2DParticleDynArray( Particle ** Particle2DArray, int NumP, int dimension){
    for (int pi = 0; pi < NumP; ++pi) {
        for (int di = 0; di < dimension; ++di) {
            Particle2DArray[pi][di].X = pi + di;
            Particle2DArray[pi][di].V = pi + di + 0.4;
        }
    }
}
void Print2DParticleDynArray( Particle ** Particle2DArray, int NumP, int dimension){
    for (int pi = 0; pi < NumP; ++pi) {
        for (int di = 0; di < dimension; ++di) {
            printf("Particle2DArray[%d][%d].X = %f\t", pi, di, Particle2DArray[pi][di].X);
            printf("Particle2DArray[%d][%d].V = %f\n", pi, di, Particle2DArray[pi][di].V);
        }
        printf("\n");
    }
}
//Dynamic 2D array END END END END END END END END END END END END



int main() {
    // EXAMPLE for 1D static arrays ------>------->------>------->------>------->
    double Double1DArray_1[XLen_1];
    double Double1DArray_2[XLen_2];
    
    initDouble1DArray( Double1DArray_1, XLen_1);
    initDouble1DArray( Double1DArray_2, XLen_2);
    
    printf("\n\nDouble1DArray_1, len = %d ----->\n", XLen_1);
    PrintDouble1DArray( Double1DArray_1, XLen_1);
    printf("\n\nDouble1DArray_2, len = %d ----->\n", XLen_2);
    PrintDouble1DArray( Double1DArray_2, XLen_2);
    // END END END END END END EXAMPLE for 1D static arrays
    
    // EXAMPLE for 2D static arrays ------>------->------>------->------>------->
    int Int2DArray_1[XLen_1][YLen];
    int Int2DArray_2[XLen_2][YLen];
    
    initInt2DArray(  Int2DArray_1, XLen_1);
    initInt2DArray(  Int2DArray_2, XLen_2);
    
    printf("\n\nInt2DArray_1----->, lenX = %d lenY = %d ----->\n", XLen_1, YLen);
    PrintInt2DArray( Int2DArray_1, XLen_1);
    printf("\n\nInt2DArray_2----->, lenX = %d lenY = %d ----->\n", XLen_2, YLen);
    PrintInt2DArray( Int2DArray_2, XLen_2);
    // END END END END END END EXAMPLE for 1D static arrays
    
    // EXAMPLE for 1D dynamic array ------>------->------>------->------>------->
    int SizeTom = 4;
    int * Tom = (int *) malloc( SizeTom * sizeof( int ) );
    if (Tom == NULL) {
        printf("Allocation of Tom failed.\nTerminate code.\n");
        return 0;
    }
    
    init1DIntDynArray(  Tom, SizeTom);
    
    printf("\n\nTom----->, SizeTom = %d ----->\n", SizeTom);
    Print1DIntDynArray(  Tom, SizeTom);
    
    free(Tom);
    // END END END END END END EXAMPLE for 1D dynamic array
    
    
    // EXAMPLE for 1D dynamic array self-defined type------>------->------>------->------>------->
    Particle P1; // Example how to use particle
    P1.X = -2.2;
    P1.V = -1.2;
    
    int SizeMarry2 = 2;
    Particle * Marry2 = (Particle *) malloc( SizeMarry2 * sizeof( Particle ) );
    if (Marry2 == NULL) {
        printf("Allocation of Marry2 failed.\nTerminate code.\n");
        return 0;
    }
    
    init1DParticleDynArray(  Marry2, SizeMarry2);
    Marry2[0] = P1;//<<-----<<-----<<-----<<-----
    
    printf("\n\nMarry2-----> (Marry2[0] is different, see code), SizeMarry2 = %d ----->\n", SizeMarry2);
    Print1DParticleDynArray(  Marry2, SizeMarry2);
    
    free(Marry2);
    // END END END END END END EXAMPLE for 1D dynamic array self-defined type
    
    
    // EXAMPLE for 2D dynamic array self-defined type------>------->------>------->------>------->
    int NumberOfParticles = 4, Dim = 2;
    Particle ** Harry = (Particle **) malloc( NumberOfParticles * sizeof( Particle * ) );
    if (Harry == NULL) {
        printf("Allocation of Harry failed.\nTerminate code.\n");
        return 0;
    }
    for (int pi = 0; pi < NumberOfParticles; ++pi) {
        Harry[pi] = (Particle *) malloc( Dim * sizeof( Particle  ) );
        if (Harry[pi] == NULL) {
            printf("Allocation of Harry[%d] failed.\nTerminate code.\n", pi);
            return 0;
        }
    }
    
    init2DParticleDynArray(  Harry, NumberOfParticles, Dim);
    
    printf("\n\nHarry-----> NumberOfParticles = %d Dim = %d ----->\n", NumberOfParticles, Dim);
    Print2DParticleDynArray(  Harry, NumberOfParticles, Dim);
    
    for (int pi = 0; pi < NumberOfParticles; ++pi) {
        free(Harry[pi]);
    }
    free(Harry);
    // END END END END END END EXAMPLE for 2D dynamic array self-defined type
    
    
    return 0;
}
