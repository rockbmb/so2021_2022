default: pessoas

pessoas.o: pessoas.c pessoa.h
	cc -c pessoas.c -o pessoas.o

pessoas: pessoas.o pessoa.o
	cc pessoas.o pessoa.o -o pessoas

pessoa.o: pessoa.c pessoa.h
	cc -c pessoa.c

clean:
	-rm pessoas
	-rm *.o