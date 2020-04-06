#include <stdio.h>
#include <jpeglib.h>

#include "jpeg.h"
#include "local.h"

/*
 * Fonction permettant de recupere la couleur d'un pixel
 */

int 
JPEG_getpixel (	int i, 
	   	int j, 
		long int image)

{
	JPEG_BUFFER * buf = (JPEG_BUFFER *)image;
	JSAMPLE * b;
	int R, G, B, ret = 0;

	if (buf != NULL &&
	    buf->image_buffer != NULL &&
	    i >= 0 && i < buf->image_width && 
	    j >= 0 && j < buf->image_height) {
		b = buf->image_buffer + (j * buf->image_width + i) * 3;
		R = *b;
		b++;
		G = *b;
		b++;
		B = *b;
		b++;
		ret = 1;
	}

	if (ret == 1)
		return R + 256 * G + 65536 * B;
	else
		return -1;
}

