CFLAGS = -Wall

default: passwd

passwd: passwd.o readln2.o
	cc $(CFLAGS) passwd.o readln2.o -o passwd

readln2.o: ../../guiao1/ex3,4,5/readln2.c ../../guiao1/ex3,4,5/readln2.h
	cc $(CFLAGS) -c ../../guiao1/ex3,4,5/readln2.c -o readln2.o

passwd.o: passwd.c ../../guiao1/ex3,4,5/readln2.h
	cc $(CFLAGS) -c passwd.c -o passwd.o

clean:
	-rm passwd *.out
	-rm *.o