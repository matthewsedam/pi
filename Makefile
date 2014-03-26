all:
	g++ --std=c++11 -Os src/*.cpp -o bin/pi -lgmp -lgmpxx

install:
	install bin/pi /usr/bin/pi
