CC=g++
CFLAGS=--std=c++11
LIBS=-lzmq -lpthread

all: Client Server

Client: Client.cpp
		$(CC) $(CFLAGS) Client.cpp -o Client.out $(LIBS)

Server: Server.cpp
		$(CC) $(CFLAGS) Server.cpp -o Server.out $(LIBS)