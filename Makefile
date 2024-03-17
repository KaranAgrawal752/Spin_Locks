all:
	g++ -pthread TASLock.cpp TTASLock.cpp ALock.cpp CLHLock.cpp MCSLock.cpp main.cpp -o program
	./program


clean:
	rm -f  program  *.o
