CFLAGS = -Wall

default: passwd3

passwd3: passwd3.o readln2.o
	cc $(CFLAGS) passwd3.o readln2.o -o passwd3

readln2.o: ../../guiao1/ex3,4,5/readln2.c ../../guiao1/ex3,4,5/readln2.h
	cc $(CFLAGS) -c ../../guiao1/ex3,4,5/readln2.c -o readln2.o

passwd3.o: passwd3.c ../../guiao1/ex3,4,5/readln2.h
	cc $(CFLAGS) -c passwd3.c -o passwd3.o

clean:
	-rm passwd3 *.out
	-rm *.o