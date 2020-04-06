#include <stdio.h>
#include <jpeglib.h>

#include "jpeg.h"
#include "local.h"

/*
 * Fonction d'allumage d'un pixel avec une couleur specifiee
 */

void 
JPEG_setpixel (int i, 
		   int j, 
		   int R, 
		   int G, 
		   int B, 
		   long int image)

{
	JPEG_BUFFER * buf = (JPEG_BUFFER *)image;
	JSAMPLE * b;

	if (buf != NULL &&
	    buf->image_buffer != NULL &&
	    i >= 0 && i < buf->image_width && 
	    j >= 0 && j < buf->image_height) {
		b = buf->image_buffer + (j * buf->image_width + i) * 3;
		*b = R; 
		b++;
		*b = G; 
		b++;
		*b = B; 
		b++;
	}
}

