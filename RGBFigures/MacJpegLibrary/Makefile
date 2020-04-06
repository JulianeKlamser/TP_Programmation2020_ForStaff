MinimalExample: MinimalExample.o ./Libraries/jpeg.a
	cc -Wall -Werror -o MinimalExample MinimalExample.o ./Libraries/jpeg.a -ljpeg

MinimalExample.o: MinimalExample.c ./Libraries/jpeg.h
	cc -O -Wall -Werror -c MinimalExample.c