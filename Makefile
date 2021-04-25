#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CC = g++
CFLAGS  = -g -Wall
RM = rm

all: entropy

clean:
	rm *.o

entropy:  main.o entropy.o 
	$(CC) $(CFLAGS) -o entropy.exe main.o entropy.o

entropy.o: entropy.cpp entropy.h  
	$(CC) $(CFLAGS) -c entropy.cpp

main.o: main.cpp entropy.h 
	$(CC) $(CFLAGS) -c main.cpp

