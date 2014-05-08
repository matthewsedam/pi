all:
	gcc -std=c99 -Os src/*.c -o bin/pi -lgmp -lgmpxx -lpthread

install:
	install bin/pi /usr/bin/pi
