IDIR =../include
CC=g++
CFLAGS=-std=c++0x -Wall -I$(IDIR)
LFLAGS=-Wall
ODIR=obj

_DEPS = HTTPMessage.h HTTPRequestMessage.h HTTPResponseMessage.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = webserver.o HTTPMessage.o HTTPRequestMessage.o HTTPResponseMessage.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

webserver: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 