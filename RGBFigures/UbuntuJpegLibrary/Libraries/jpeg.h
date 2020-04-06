int
JPEG_save_file (const char * filename,
                long int image);

long int 
JPEG_init_image (int width, 
		 int height,
                 int R,
		 int G,
		 int B);

int 
JPEG_destroy_image (long int image);

void 
JPEG_setpixel (int i, 
	       int j, 
               int R,
	       int G,
	       int B,
               long int image);

