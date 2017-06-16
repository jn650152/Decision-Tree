CC=g++
CFLAGS= -std=c++0x -I.

assign4: functions.cpp main.cpp
	$(CC) -o assign4 functions.cpp main.cpp $(CFLAGS).
