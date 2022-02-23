default: mynl

readln1.o: readln1.c readln1.h
	cc -c readln1.c

mynl.o: mynl.c readln1.h
	cc -c mynl.c -o mynl.o

mynl: mynl.o readln1.o
	cc mynl.o readln1.o -o mynl

readln2.o: readln2.c readln2.h
	cc -c readln2.c

mynl2.o: mynl2.c readln2.h
	cc -c mynl2.c -o mynl2.o

mynl2: mynl2.o readln2.o
	cc mynl2.o readln2.o -o mynl2

clean:
	-rm mynl mynl2
	-rm *.o