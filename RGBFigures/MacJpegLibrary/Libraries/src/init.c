#include <stdio.h>
#ifdef __APPLE__
#include <stdlib.h>
#else
#include <malloc.h>
#endif
#include <jpeglib.h>

#include "jpeg.h"
#include "local.h"

/*
 * Fonction d'initialisation pour trace JPEG
 * width et height correspondent a la taille de l'image en pixels
 * Retour: pointeur (void *), NULL si probleme
 */

long int 
JPEG_init_image (int width, 
		 int height,
		 int R,
		 int G,
		 int B)

{
	JPEG_BUFFER * buf = NULL;
	JSAMPLE * b;
	int i, j;

	if (height > 0 && width > 0) {
		buf = (JPEG_BUFFER *)malloc(sizeof(JPEG_BUFFER));
		if (buf != NULL) {
			buf->image_buffer = (JSAMPLE *)malloc(height * width * 3 * sizeof(JSAMPLE));
			b = buf->image_buffer;

			if (buf->image_buffer == NULL) {
				free(buf);
				buf = NULL;
			}
			else {
				buf->image_height = height;
				buf->image_width = width;
				for (i = 0; i < width; i++) {
					for (j = 0; j < height; j++) {
						*b = R; 
						b++;   
						*b = G; 
						b++;   
						*b = B; 
						b++;   
					}
				}
			}
		}

	}

	return (long int)buf;
}

