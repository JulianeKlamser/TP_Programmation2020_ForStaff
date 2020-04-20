#include <stdlib.h>
#include <stdio.h>
#define TotalPixX 200
#define TotalPixY 100
#define FileNameLength 99

typedef struct{
    int Red;
    int Green;
    int Blue;
}RGB;

int WriteImageDataToFile (char *FileName, RGB ImageData[][TotalPixY], int TotPixX, int TotPixY){
    int TESTopen = 1; // 1 means successful
    FILE *fp;
    fp = fopen( FileName, "w"); //open file in write ("w") modus
    if (fp == NULL ) { //file could not be opened
        TESTopen = 0; // zero means NOT successful
    }
    else{
        for (int x = 0; x < TotPixX; x++) {
            for (int y = 0; y < TotPixY; ++y) {
                //write to file
                fprintf(fp, "%d %d %d %d %d\n",
                        x,
                        y,
                        ImageData[x][y].Red,
                        ImageData[x][y].Green,
                        ImageData[x][y].Blue);
            }
        }
        fclose(fp);//close File
    }
    return TESTopen;
}

int main(){
    int TESTopen = 1; // 1 means successful
    
    RGB PixelData[TotalPixX][TotalPixY]; // this is a 2D array (not of type double or int but) of type RGB
    
    for (int x = 0; x < TotalPixX; x++) {
        for (int y = 0; y < TotalPixY; ++y) {
            PixelData[x][y].Red = 0; //zero intensity
            PixelData[x][y].Green = 255; //full intensity
            PixelData[x][y].Blue = 100; //some intensity
        }
    }
    char FileName[FileNameLength] = "./DataInitialImage.txt";
    TESTopen = WriteImageDataToFile ( FileName, PixelData, TotalPixX, TotalPixY);
    if ( TESTopen == 0) {
        printf("\nFile %s could not be opened.\nCode is terminated\n\n", FileName);
        return 0;
    }
      
    // draw red horizontal lines and save pictures
    int pictureID = 0;
    for (int y = 10; y < TotalPixY; y += 20) {
        for (int x = 0; x < TotalPixX; x++) {
            PixelData[x][y].Red = 255; //full intensity
            PixelData[x][y].Green = 0; //no green
            PixelData[x][y].Blue = 0; //no blue
        }
        snprintf(FileName, FileNameLength, "./DataChangedImage_%d.txt", pictureID++);
        TESTopen = WriteImageDataToFile ( FileName, PixelData, TotalPixX, TotalPixY);
        if ( TESTopen == 0) {
            printf("\nFile %s could not be opened.\nCode is terminated\n\n", FileName);
            return 0;
        }
    }
  
    return 0;
}




