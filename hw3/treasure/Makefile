all: test

treasure:
	g++ -std=c++11 -Wall treasure.cpp -o treasure

debug:
	g++ -std=c++11 -Wall treasure.cpp -DDEBUG -o treasure

test: test1 test2 test3 test4

test1: treasure
	time ./treasure < input1

test2: treasure
	time ./treasure < input2

test3: treasure
	time ./treasure < input3

test4: treasure
	time ./treasure < input4

mytest1: treasure
	time ./treasure < myinput1

mytest2: treasure
	time ./treasure < myinput2

clean:
	rm -rf treasure


