myprog : mytest.o
	g++ mytest.o -o myprog

mytest.o : mytest.cpp hash.h heap.h
	g++ -Wall -c mytest.cpp

clean : 
	rm *.o myprog

val1 : 
	valgrind ./myprog

run : 
	./myprog