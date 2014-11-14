CC=gcc

# For keg-only brew installed libxml2; change to fit
LDFLAGS=-lxml2 -L/usr/local/opt/libxml2/lib
CPPFLAGS=-I/usr/local/opt/libxml2/include/libxml2

all:	plget

plget:	plget.c
	$(CC) -g plget.c -o plget $(LDFLAGS) $(CPPFLAGS)

clear:
	rm plget *~
