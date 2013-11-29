OPENGL_LDFLAGS= -lGL -lGLU -lglut

CFLAGS = -std=c++0x -Wall -g 

CC = g++

all: main

clean:
	rm -f *.o main

Train.o: Train.cpp Train.h
	$(CC) -c $(CFLAGS) -o $@ $<

Station.o: Station.cpp Station.h
	$(CC) -c $(CFLAGS) -o $@ $<

splitstring.o: splitstring.cpp splitstring.h
	$(CC) -c $(CFLAGS) -o $@ $<

tools.o: tools.cpp tools.h
	$(CC) -c $(CFLAGS) -o $@ $<

main.o:	main.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

main: main.o Train.o Station.o splitstring.o tools.o
	$(CC) -o $@ $^ $(OPENGL_LDFLAGS)

