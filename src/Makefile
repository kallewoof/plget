LDFLAGS=-lxml2 -L/usr/local/opt/libxml2/lib
CPPFLAGS=-I/usr/local/opt/libxml2/include/libxml2
CC=gcc

all:	plget

plget:	plget.c
	$(CC) plget.c -o plget $(LDFLAGS) $(CPPFLAGS)

install:plget
	cp plget /usr/local/bin

clean:
	rm plget *~
