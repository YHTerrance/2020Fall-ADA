all: test

sequence:
	g++ -std=c++11 -Wall sequence.cpp -o sequence

debug:
	g++ -std=c++11 -Wall sequence.cpp -o sequence -DDEBUG

test: test1 test2 mytest1

test1: sequence
	time ./sequence < input1

test2: sequence
	time ./sequence < input2

test3: sequence
	time ./sequence < input3

test4: sequence
	time ./sequence < input4

mytest1: sequence
	time ./sequence < myinput1

clean:
	rm -rf sequence


