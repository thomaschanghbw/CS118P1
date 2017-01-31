CC=g++ 
CFLAGS=-std=c++0x -Wall -c -I.
LFLAGS=-Wall 
OBJS=webserver.o HTTPMessage.o HTTPRequestMessage.o
EXCUTABLE=webserver

$(EXCUTABLE): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(EXCUTABLE)

webserver.o: webserver.cpp
	$(CC) $(CFLAGS) webserver.cpp

HTTPMessage.o: HTTPMessage.cpp
	$(CC) $(CFLAGS) HTTPMessage.cpp

HTTPRequestMessage.o: HTTPRequestMessage.cpp
	$(CC) $(CFLAGS) HTTPRequestMessage.cpp

clean:
	$(RM) *.o *~ $(EXCUTABLE)
