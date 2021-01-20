.POSIX:
.SUFFIXES:
CC     = cc
LIBS = x11
CFLAGS = -Wall $(shell pkg-config --cflags $(LIBS))
LDLIBS = $(shell pkg-config --libs $(LIBS))
LDFLAGS = 

all: build
run: clean build
	./window

build: xfix bstring window
	$(CC) $(LDFLAGS) -o window window.o bstrlib.o xfix.o $(LDLIBS)
window:
	$(CC) -c $(CFLAGS) window.c
xfix:
	$(CC) -c $(CFLAGS) ../../src/xfix.c
bstring:
	$(CC) -c $(CFLAGS) ../../lib/bstrlib.c
clean:
	rm -f xfix.o window.o bstrlib.o window