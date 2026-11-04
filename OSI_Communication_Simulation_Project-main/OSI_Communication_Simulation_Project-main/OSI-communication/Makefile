CC = gcc
CFLAGS = -Wall

target: osi

osi: main.o layers.o
	$(CC) main.o layers.o -o osi

main.o: main.c layers.h
	$(CC) $(CFLAGS) -c main.c

layers.o: layers.c layers.h
	$(CC) $(CFLAGS) -c layers.c

clean:
	rm -f *.o osi
