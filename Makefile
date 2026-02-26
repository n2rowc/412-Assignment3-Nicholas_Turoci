CC = g++
CFLAGS = -std=c++17 -Wall

all: myprogram

myprogram: main.o LoadBalancer.o Switch.o IPBlocker.o WebServer.o RequestQueue.o
	$(CC) $(CFLAGS) main.o LoadBalancer.o Switch.o IPBlocker.o WebServer.o RequestQueue.o

main.o: src/main.cpp
	$(CC) $(CFLAGS) -c src/main.cpp

LoadBalancer.o: src/LoadBalancer.cpp
	$(CC) $(CFLAGS) -c src/LoadBalancer.cpp

Switch.o: src/Switch.cpp
	$(CC) $(CFLAGS) -c src/Switch.cpp

IPBlocker.o: src/IPBlocker.cpp
	$(CC) $(CFLAGS) -c src/IPBlocker.cpp

WebServer.o: src/WebServer.cpp
	$(CC) $(CFLAGS) -c src/WebServer.cpp

RequestQueue.o: src/RequestQueue.cpp
	$(CC) $(CFLAGS) -c src/RequestQueue.cpp

clean:
	rm -f myprogram *.o
