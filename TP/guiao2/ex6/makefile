default: matrix

matrix.o: matrix.c gen.h
	cc -c matrix.c

gen.o: gen.c gen.h
	cc -c gen.c

matrix: matrix.o gen.o
	cc matrix.o gen.o -o matrix

clean:
	-rm matrix
	-rm *.o