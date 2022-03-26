default: myshell

myshell: myshell.o mysystem.o
	cc myshell.o mysystem.o -o myshell

myshell.o: myshell.c mysystem.h
	cc -c myshell.c -o myshell.o

mysystem: mysystem.o
	cc mysystem.o -o mysystem

mysystem.o: mysystem.c mysystem.h
	cc -c mysystem.c -o mysystem.o

clean:
	-rm mysystem
	-rm *.o
	-rm a.out