#!/bin/bash
# Crude config to detect libxml2.

if [ ! -e "Makefile.in" ]; then
    echo "Missing Makefile.in -- cannot configure"
    exit 1
fi

rm -f Makefile

echo -n -e "libxml2: "

# We use keg-only brew if found otherwise built-in
if [ -d "/usr/local/opt/libxml2/lib" ] && [ -d "/usr/local/opt/libxml2/include/libxml2" ]; then
    echo "keg-only brew"
    echo -n -e "LDFLAGS=-lxml2 -L/usr/local/opt/libxml2/lib\nCPPFLAGS=-I/usr/local/opt/libxml2/include/libxml2\n" > Makefile
else
    echo "built-in"
    echo -n -e "LDFLAGS=-lxml2 -I/usr/include/libxml2\nCPPFLAGS=\n" > Makefile
fi

cat Makefile.in >> Makefile

echo "Done. Now run: make"
