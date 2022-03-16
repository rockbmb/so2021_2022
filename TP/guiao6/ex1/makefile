CFLAGS = -Wall

default: all

all: mkfifo stdin stdout

mkfifo: mkfifo.o
	cc $(CFLAGS) mkfifo.o -o mkfifo

mkfifo.o: mkfifo.c
	cc $(CFLAGS) -c mkfifo.c -o mkfifo.o

readln2.o: ../../guiao1/ex3,4,5/readln2.c ../../guiao1/ex3,4,5/readln2.h
	cc $(CFLAGS) -c ../../guiao1/ex3,4,5/readln2.c -o readln2.o

stdin: stdin.o readln2.o
	cc $(CFLAGS) stdin.o readln2.o -o stdin

stdin.o: stdin.c ../../guiao1/ex3,4,5/readln2.h
	cc $(CFLAGS) -c stdin.c -o stdin.o

stdout: stdout.o readln2.o
	cc $(CFLAGS) stdout.o readln2.o -o stdout

stdout.o: stdout.c ../../guiao1/ex3,4,5/readln2.h
	cc $(CFLAGS) -c stdout.c -o stdout.o

clean:
	-rm mkfifo stdin stdout
	-rm *.o
	unlink fifo

clean2:
	-rm *.o