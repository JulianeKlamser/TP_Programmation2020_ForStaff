#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <assert.h>
#define TotalPixX 200
#define TotalPixY 100
#define FileNameLength 99

typedef struct{
    int Red;
    int Green;
    int Blue;
}RGB;

void WriteImageDataToFile (char *FileName, RGB ImageData[][TotalPixY], int TotPixX, int TotPixY){
    FILE *fp;
    fp = fopen( FileName, "w"); //open file
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

int main(){
  
    RGB PixelData[TotalPixX][TotalPixY];
    
    for (int x = 0; x < TotalPixX; x++) {
        for (int y = 0; y < TotalPixY; ++y) {
            PixelData[x][y].Red = 0; //zero intensity
            PixelData[x][y].Green = 255; //full intensity
            PixelData[x][y].Blue = 100; //some intensity
        }
    }
    char FileName[FileNameLength] = "./DataInitialImage.txt";
    WriteImageDataToFile ( FileName, PixelData, TotalPixX, TotalPixY);
      
    // draw red horizontal lines and save pictures
    int pictureID = 0;
    for (int y = 10; y < TotalPixY; y += 20) {
        for (int x = 0; x < TotalPixX; x++) {
            PixelData[x][y].Red = 255; //full intensity
            PixelData[x][y].Green = 0; //no green
            PixelData[x][y].Blue = 0; //no blue
        }
        snprintf(FileName, FileNameLength, "./DataChangedImage_%d.txt", pictureID++);
        WriteImageDataToFile ( FileName, PixelData, TotalPixX, TotalPixY);
    }
  
    return 0;
}




