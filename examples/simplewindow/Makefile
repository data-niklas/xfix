.POSIX:
.SUFFIXES:
CC     = cc
LIBS = x11
CFLAGS = -Wall
LDLIBS = 
LDFLAGS = 

all: build
run: clean build
	./window

build: xfix window
	$(CC) $(LDFLAGS) -o window window.o xfix.o $(LDLIBS)
window:
	$(CC) -c $(CFLAGS) window.c
xfix:
	$(CC) -c $(CFLAGS) ../../src/xfix.c
clean:
	rm -f xfix.o window.o window