default: pessoas

pessoas.o: pessoas.c
	cc -c pessoas.c -o pessoas.o

pessoas: pessoas.o
	cc pessoas.o -o pessoas

clean:
	-rm pessoas
	-rm *.o