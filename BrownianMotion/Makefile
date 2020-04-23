main: main.o
	cc -o main main.o -lm

main.o: main.c
	cc -c main.c -Wall -Wextra -Werror -Wshadow -O -pedantic

clean :
	rm *.o main *.dat *.png