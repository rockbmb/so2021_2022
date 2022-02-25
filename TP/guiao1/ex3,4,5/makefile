# Primeira versão de readln:
#make SLOW=1
# Segunda version de readln:
#make

default: mynl

readln1.o: readln1.c readln1.h
	cc -c readln1.c

readln2.o: readln2.c readln2.h
	cc -c readln2.c

ifdef SLOW
mynl.o: mynl.c readln1.h
	cc -c mynl.c -o mynl.o -DSLOW

mynl: mynl.o readln1.o
	cc mynl.o readln1.o -o mynl -DSLOW
else
mynl.o: mynl.c readln2.h
	cc -c mynl.c -o mynl.o

mynl: mynl.o readln2.o
	cc mynl.o readln2.o -o mynl
endif

clean:
	-rm mynl
	-rm *.o