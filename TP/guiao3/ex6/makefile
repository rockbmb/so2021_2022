CFLAGS = -Wall

default: controlador a.out b.out c.out

controlador: controlador.o
	cc controlador.o -o controlador

controlador.o: controlador.c
	cc $(CFLAGS) -c controlador.c -o controlador.o

a.out: auxiliar.c
	cc $(CFLAGS) auxiliar.c -o a.out

b.out: auxiliar.c
	cc $(CFLAGS) auxiliar.c -o b.out

c.out: auxiliar.c
	cc $(CFLAGS) auxiliar.c -o c.out

clean:
	-rm controlador *.out
	-rm *.o