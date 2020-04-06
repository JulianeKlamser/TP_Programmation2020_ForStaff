#include <stdio.h>
#ifdef __APPLE__
#include <stdlib.h>
#else
#include <malloc.h>
#endif
#include <jpeglib.h>

#include "local.h"

/*
 * Fonction de destruction des buffers pour le trace d'images JPEG
 */

void 
JPEG_destroy_image (long int image)

{
	JPEG_BUFFER * buf = (JPEG_BUFFER *)image;

	if (buf != NULL) {
		if (buf->image_buffer != NULL) 
			free(buf->image_buffer);
		free(buf);
		buf = NULL;
	}
}

