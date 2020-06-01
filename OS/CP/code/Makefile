all: player server

player : player.c functions.o
	gcc -g3 -Wall player.c functions.o -lzmq -o player -pthread

server: functions.o server.c
	gcc -g3 -Wall server.c functions.o -lzmq -o server

functions.o : functions.c functions.h
	gcc -c functions.c

clean:
	rm -f *.o player server