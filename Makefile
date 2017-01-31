CC=g++ 
CFLAGS=-std=c++0x -Wall -c -I.
LFLAGS=-Wall 
OBJS=webserver.o HTTPMessage.o
EXCUTABLE=webserver

$(EXCUTABLE): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(EXCUTABLE)

webserver.o: webserver.cpp
	$(CC) $(CFLAGS) webserver.cpp 

HTTPMessage.o: HTTPMessage.cpp
	$(CC) $(CFLAGS) HTTPMessage.cpp

clean:
	$(RM) *.o *~ $(EXCUTABLE)
