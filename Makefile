CC=g++ 
CFLAGS=-std=c++0x -Wall -c
LFLAGS=-Wall 
OBJS=webserver.o
EXCUTABLE=webserver

$(EXCUTABLE): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(EXCUTABLE)

webserver.o: webserver.cpp
	$(CC) $(CFLAGS) webserver.cpp 

clean:
	$(RM) *.o *~ $(EXCUTABLE)
