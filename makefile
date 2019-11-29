CC = g++
LD = g++
CCFLAGS = -Wall -pedantic  -std=c++11
LDFLAGS =

main.out: main.cpp server.out
	$(LD) $(CCFLAGS) -o main.out main.cpp
server.out: server.o lin-2-list-barrier.o
	$(LD) $(LDFLAGS) -o server.out server.o lin-2-list-barrier.o
server.o: server.cpp lin-2-list-barrier.h
	$(CC) $(CCFLAGS) -c server.cpp
lin-2-list-barrier.o: lin-2-list-barrier.c lin-2-list-barrier.h
	$(CC) $(CCFLAGS) -c lin-2-list-barrier.c
clean:
	rm *.o
