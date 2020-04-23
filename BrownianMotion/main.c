#include <stdio.h>
#include <math.h>
#include <time.h> // for wall time
#include <stdlib.h> //for random numbers

typedef struct{
    double x, y;
}Walker;

double ** DynamicallyAllocateInt2DSquareArray( unsigned Size ){
    double ** Array;
    Array = (double**) malloc( Size * sizeof(double *));//only malloc possible!!!
    if( Array == NULL ){
        printf("-1 Memory allocation failed\n");
        exit(0);
    }
    for( unsigned xIndex = 0; xIndex < Size; xIndex++){
        Array[xIndex] = (double*) calloc(Size , sizeof(double));
        if(Array[xIndex] == NULL){
            printf("%d Memory allocation failed\n", xIndex);
            exit(0);
        }
    }
    return Array;
}

void Init2DSquareHistToZero( double ** Hist, int HistSize ){
    for (int x_i = 0; x_i < HistSize; ++x_i) {
        for (int y_i = 0; y_i < HistSize; ++y_i) {
            Hist[x_i][y_i] = 0.0;
        }
    }
}

double RandomUniform ( double Min, double Max ){
    double RandDouble = (double) rand() / RAND_MAX; //between 0.0 et 1.0
    double Diff = Max - Min;
    return (Diff * RandDouble) + Min;
}

int WriteImageDataToFile (char *FileName, double ** Histogram, int HistSize){
    int TESTopen = 0; // 0 means NO problem
    FILE *fp;
    fp = fopen( FileName, "w"); //open file in write ("w") modus
    if (fp == NULL ) { //file could not be opened
        TESTopen = 1; // 1 means Problem
    }
    else{
        double MaxValue = 0.0;
        for (int x_i = 0; x_i < HistSize; x_i++) {
            for (int y_i = 0; y_i < HistSize; ++y_i) {
                if (Histogram[x_i][y_i] > MaxValue) {
                    MaxValue = Histogram[x_i][y_i];
                }
            }
        }
        int MaxIntensity = 225;
        int Red, Green, Blue;
        for (int x_i = 0; x_i < HistSize; x_i++) {
            for (int y_i = 0; y_i < HistSize; ++y_i) {
                Red =  ( Histogram[x_i][y_i] / MaxValue ) * MaxIntensity;
                Green =  ( Histogram[x_i][y_i] / MaxValue ) * MaxIntensity;
                Blue =  ( Histogram[x_i][y_i] / MaxValue ) * MaxIntensity;
                fprintf(fp, "%d %d %d %d %d\n",
                        x_i,
                        y_i,
                        Red,
                        Green,
                        Blue);
            }
        }
        fclose(fp);
    }
    return TESTopen;
}

int WriteY0DataToFile ( char *FileName, double ** Histogram, int HistSize, double BinWidth, double MaxDist){
    int TESTopen = 0; // 0 means NO problem
    FILE *fp;
    fp = fopen( FileName, "w"); //open file in write ("w") modus
    if (fp == NULL ) { //file could not be opened
        TESTopen = 1; // means problem
    }
    else{
        double Sum = 0.0;
        for (int x_i = 0; x_i < HistSize; x_i++) {
            double Probability = 0.0;
            for (int y_i = 0; y_i < HistSize; y_i++) {//integration over y
                Probability += Histogram[x_i][y_i] * BinWidth;
            }
            fprintf(fp, "%f %f\n", -MaxDist + (0.5+x_i)*BinWidth, Probability);
            Sum += BinWidth*Probability;
        }
        fclose(fp);//close File
        printf("Sum = %f\n", Sum);
    }
    return TESTopen;
}

int WriteTheoryDataToFile( char *FileName, int time, double MaxDist, double  MaxDispl){
    int TESTopen = 0; // 0 means No probelm
    FILE *fp;
    fp = fopen( FileName, "w"); //open file in write ("w") modus
    if (fp == NULL ) { //file could not be opened
        TESTopen = 1; // means problem
    }
    else{
        double Sum = 0.0;
        double helper = 0.0;
        double C = (2./3.) * pow(MaxDispl,2) * time;
        for (double x = - MaxDist; x < MaxDist; x += 0.1) {
            helper = exp( - pow(x,2) / C ) / sqrt(C * M_PI);
            Sum += helper*0.1;
            fprintf(fp, "%f %f\n", x, helper );
        }
        fclose(fp);//close File
        printf("Sum = %f\n", Sum);
    }
    return TESTopen;
}

int AppendToDataFile( char *FileName, double X, double  Y, int index){
    int TESTopen = 0; // 0 means No probelm
    FILE *fp;
    if (index == 0) fp = fopen( FileName, "w"); //open file in write ("w") modus
    else fp = fopen( FileName, "a"); // append
        
    if (fp == NULL ) { //file could not be opened
        TESTopen = 1; // one means problem
    }
    else{
        fprintf(fp, "%f %f\n", X, Y );
        fclose(fp);//close File
    }
    return TESTopen;
}

int main() {
    srand((unsigned) time(0));
   
    int HistSize = 200;
    double ** Histogram = DynamicallyAllocateInt2DSquareArray( HistSize );
    Init2DSquareHistToZero( Histogram, HistSize );
    int x_i, y_i;
    
    int MaxTime = 10;
    double MaxDispl = 1.0;
    double MaxDist = MaxDispl * MaxTime;
    double TotalArenaSize = 2. * MaxDist;
    double BinWidth = TotalArenaSize / HistSize ;
    

    Walker Part;
    int TotalExperiments = pow(10,7);
    for (int experiment_i = 0; experiment_i < TotalExperiments; experiment_i++) {
        Part.x = 0.0;
        Part.y = 0.0;
        for (int t_i = 0; t_i < MaxTime; ++t_i) {
            if (experiment_i == 0){//Write trajectory of first experiment to data file, create new file, then append file
                if(AppendToDataFile( "./RandomWalk.dat", Part.x, Part.y, t_i)) printf( "Writing to ./RandomWalk.dat has failed\n" );
            }
            Part.x += RandomUniform ( -MaxDispl, MaxDispl );//rand() % 3 - 1; //////
            Part.y += RandomUniform ( -MaxDispl, MaxDispl );//rand() % 3 - 1;//////
        }
        x_i = ( Part.x + MaxDist ) / BinWidth;
        y_i = ( Part.y + MaxDist ) / BinWidth;
        if ( x_i >= HistSize || y_i >= HistSize ) {
            printf("MaxDist = %f too small\n", MaxDist);
            for (x_i = 0; x_i < HistSize; ++x_i) free(Histogram[x_i]);
            free(Histogram);
            return 0;
        }
        ++Histogram[x_i][y_i];
    }
    
    //normalize
    double normalization = TotalExperiments * (BinWidth * BinWidth);
    for ( x_i = 0; x_i < HistSize; ++x_i) {
        for ( y_i = 0; y_i < HistSize; ++y_i){
            Histogram[ x_i ][ y_i ] /= normalization;
        }
    }
    if ( WriteImageDataToFile ("./Histogram.dat", Histogram, HistSize) ) printf( "Data saving failed\n");
    
    if ( WriteY0DataToFile ( "./PofX.dat", Histogram, HistSize, BinWidth, MaxDist) ) printf( "Data saving failed\n");
    
    if ( WriteTheoryDataToFile("./PofXTheory.dat", MaxTime,  MaxDist,  MaxDispl)) printf( "Data saving failed\n");
    
    
    for (x_i = 0; x_i < HistSize; ++x_i) {
        free(Histogram[x_i]);
    }
    free(Histogram);
    return 0;
}
