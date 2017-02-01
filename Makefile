CC=g++ 
CFLAGS=-std=c++0x -Wall -c -I.
LFLAGS=-Wall 
OBJS=webserver.o HTTPMessage.o HTTPRequestMessage.o HTTPResponseMessage.o
EXCUTABLE=webserver

$(EXCUTABLE): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(EXCUTABLE)

webserver.o: webserver.cpp
	$(CC) $(CFLAGS) webserver.cpp

HTTPMessage.o: HTTPMessage.cpp HTTPMessage.h
	$(CC) $(CFLAGS) HTTPMessage.cpp

HTTPRequestMessage.o: HTTPRequestMessage.cpp HTTPRequestMessage.h
	$(CC) $(CFLAGS) HTTPRequestMessage.cpp

HTTPResponseMessage.o: HTTPResponseMessage.cpp HTTPResponseMessage.h
	$(CC) $(CFLAGS) HTTPResponseMessage.cpp

clean:
	$(RM) *.o *~ $(EXCUTABLE)
