Concurrency.exe: concurrency.o
	gcc -g -Wall concurrency.o -o Concurrency.exe
concurrency.o: testrand.c mt19937ar.c
	gcc -o concurrency.o testrand.c mt19937ar.c
clean:
	rm *.o *.out *.exe
