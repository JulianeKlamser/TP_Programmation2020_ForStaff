#ifndef _LOCAL_H_INCLUDED

#define _LOCAL_H_INCLUDED

/*
 * Structure de donnee interne
 */

typedef struct _JPEG_BUFFER {

	JSAMPLE * image_buffer;		/* large array of R,G,B-order data */
	int image_height;		/* number of rows in image */
	int image_width;		/* number of columns in image */

} JPEG_BUFFER;

#endif

