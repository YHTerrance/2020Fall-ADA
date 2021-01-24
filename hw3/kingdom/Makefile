all: test

kingdom:
	g++ -std=c++11 -Wall kingdom.cpp -o kingdom

debug:
	g++ -std=c++11 -Wall kingdom.cpp -DDEBUG -o kingdom

test: test1 test2 test3

test1: kingdom
	time ./kingdom < input1

test2: kingdom
	time ./kingdom < input2

test3: kingdom
	time ./kingdom < input3

mytest1: kingdom
	time ./kingdom < myinput1

mytest2: kingdom
	time ./kingdom < myinput2

clean:
	rm -rf kingdom


