all:server cilent
server:SocketServer.o
	gcc SocketServer.o -o server
SocketServer.o:SocketServer.c
	gcc -c SocketServer.c

cilent:SocketCilent.o
	gcc SocketCilent.o -o cilent
SocketCilent.o:SocketCilent.c
	gcc -c SocketCilent.c

clean:
	rm *.o
