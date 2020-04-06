#include "./Libraries/jpeg.h" //for jpeg manipulation
#include <stdio.h> //for printf

int main(void){
    // see here for RGB color picker https://www.w3schools.com/colors/colors_rgb.asp
    
    // Initialize the image
    int TotPixX = 200, TotPixY = 300; // total number of pixel in x and y direction
    int Red = 255, Green = 255, Blue = 255; // full intensity on red, green and blue
    long int ImageAddress = JPEG_init_image ( TotPixX, TotPixY, Red, Green, Blue);
    
    // Example of how to modify the image
    int pixIndexX, pixIndexY = 0.1 * TotPixY;
    Red = 2; // lower intensity for red
    for ( pixIndexX = 0; pixIndexX < TotPixX; ++pixIndexX ){
        // draw horizontal line
        JPEG_setpixel ( pixIndexX, pixIndexY, Red, Green, Blue, ImageAddress);
    }
    
    // Save the image
    char filename[64] = "./HoizontalLine.jpg";
    int test = JPEG_save_file (filename, ImageAddress);
    if (test == 1) {
        printf( "\nThe image is saved in the same folder as MinimalExample.c under the name %s\n\n", filename);
    }
    else{
        printf( "The image was not saved.\n");
    }
    
    // free the memory occupied for the image
    JPEG_destroy_image ( ImageAddress );
    
    return 0;
}
