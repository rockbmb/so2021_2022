default: mynl

readln1.o: readln1.c readln1.h
	cc -c readln1.c

mynl.o: mynl.c readln1.h
	cc -c mynl.c -o mynl.o

mynl: mynl.o readln1.o
	cc mynl.o readln1.o -o mynl

clean:
	-rm mynl
	-rm *.o