#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "jpeg.h"

int
main (void)

{
	long int image;
	int res, width = 300, height = 250;
	int i, j, R, G, B;

	image = JPEG_init_image(width, height, 0, 0, 0);
	if (image == 0) {
		printf("initialisation image impossible\n");
		return 0;
	}

	srand(time(NULL));
	for (i = 0; i < width; i++) {
		for (j = 0; j < height; j++) {
			R = rand() % 256;
			G = rand() % 256;
			B = rand() % 256;
			JPEG_setpixel(i, j, R, G, B, image);
		}
	}

	res = JPEG_save_file("test.jpg", image);
	if (res == 0) {
		printf("erreur de sauvegarde de l'image\n");
	}

	JPEG_destroy_image(image);

	return 0;
}

