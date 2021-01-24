all: test

segments:
	g++ -Wall segments.cpp -o segments

debug:
	g++ -std=c++11 -Wall segments.cpp -DDEBUG -o segments

test: test1 test2 test3 test4

test1: segments
	time ./segments < input1

test2: segments
	time ./segments < input2

test3: segments
	time ./segments < input3

test4: segments
	time ./segments < input4

mytest1: segments
	time ./segments < myinput1

mytest2: segments
	time ./segments < myinput2

clean:
	rm -rf segments


