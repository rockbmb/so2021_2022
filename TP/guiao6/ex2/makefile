CFLAGS = -Wall

default: all

all: server client

server: server.o
	cc $(CFLAGS) server.o -o server

server.o: server.c
	cc $(CFLAGS) -c server.c -o server.o

client: client.o
	cc $(CFLAGS) client.o -o client

client.o: client.c
	cc $(CFLAGS) -c client.c -o client.o

clean:
	-rm server client
	-rm *.o

clean2:
	-rm *.o