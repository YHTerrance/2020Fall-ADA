all: test

roads:
	g++ -std=c++11 -Wall roads.cpp -o roads

debug:
	g++ -std=c++11 -Wall roads.cpp -DDEBUG -o roads

test: test1 test2 test3 test4

test1: roads
	time ./roads < input1

test2: roads
	time ./roads < input2

test3: roads
	time ./roads < input3

test4: roads
	time ./roads < input4

mytest1: roads
	time ./roads < myinput1

mytest2: roads
	time ./roads < myinput2

clean:
	rm -rf roads


