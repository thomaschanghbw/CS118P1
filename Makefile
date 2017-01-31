CC=g++ 
CFLAGS=-std=c++0x -Wall -c
LFLAGS=-Wall 
OBJS=webserver.o request_handler.o
EXCUTABLE=webserver

$(EXCUTABLE): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(EXCUTABLE)

webserver.o: webserver.cpp request_handler.h
	$(CC) $(CFLAGS) webserver.cpp request_handler.h

request_handler.o: request_handler.cpp request_handler.h
	$(CC) $(CFLAGS) request_handler.cpp request_handler.h 

clean:
	$(RM) *.o *~ $(EXCUTABLE)
