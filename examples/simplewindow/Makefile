.POSIX:
.SUFFIXES:
# Xinerama, comment to disable
XINERAMALIB = xinerama
XINERAMAFLAG = -DXINERAMA

CC     = cc
LIBS = x11 imlib2 appindicator-0.1 ${XINERAMALIB}
CFLAGS = -Wall $(shell pkg-config --cflags $(LIBS)) ${XINERAMAFLAG} -g
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