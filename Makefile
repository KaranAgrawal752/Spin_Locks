all:
	g++ -pthread TASLock.cpp TTASLock.cpp Backoff.cpp BackoffLock.cpp ALock.cpp CLHLock.cpp MCSLock.cpp TOLock.cpp main.cpp -o program


clean:
	rm -f  program  *.o
