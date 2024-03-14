all:
	g++ -pthread TASLock.cpp TTASLock.cpp ALock.cpp CLHLock.cpp MCSLock.cpp main.cpp -o program


clean:
	rm -f  program  *.o
