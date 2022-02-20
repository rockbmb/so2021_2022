default: mycp

mycp.o: mycp.c
	cc -c mycp.c -o mycp.o

mycp: mycp.o
	cc mycp.o -o mycp

clean:
	-rm mycp
	-rm *.o