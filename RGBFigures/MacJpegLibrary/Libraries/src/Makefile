OBJS = write.o destroy.o setpixel.o init.o getpixel.o

test: test.o jpeg.a
	cc -o test test.o jpeg.a -ljpeg

test.o: test.c jpeg.h
	cc -c test.c -Wall  -O

jpeg.a: $(OBJS)
	ar rv /tmp/jpeg.a $(OBJS)
	cp /tmp/jpeg.a . ; rm -f /tmp/jpeg.a

write.o: write.c jpeg.h local.h
	cc -c write.c -Wall  -O -I$(HOME)/libs/src/include

destroy.o: destroy.c jpeg.h local.h
	cc -c destroy.c -Wall  -O -I$(HOME)/libs/src/include

getpixel.o: getpixel.c jpeg.h local.h
	cc -c getpixel.c -Wall  -O -I$(HOME)/libs/src/include

setpixel.o: setpixel.c jpeg.h local.h
	cc -c setpixel.c -Wall  -O -I$(HOME)/libs/src/include

init.o: init.c jpeg.h local.h
	cc -c init.c -Wall  -O -I$(HOME)/libs/src/include

