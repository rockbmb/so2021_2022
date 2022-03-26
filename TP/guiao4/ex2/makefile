CFLAGS = -Wall

default: passwd2

passwd2: passwd2.o readln2.o
	cc $(CFLAGS) passwd2.o readln2.o -o passwd2

readln2.o: ../../guiao1/ex3,4,5/readln2.c ../../guiao1/ex3,4,5/readln2.h
	cc $(CFLAGS) -c ../../guiao1/ex3,4,5/readln2.c -o readln2.o

passwd2.o: passwd2.c ../../guiao1/ex3,4,5/readln2.h
	cc $(CFLAGS) -c passwd2.c -o passwd2.o

clean:
	-rm passwd2 *.out
	-rm *.o