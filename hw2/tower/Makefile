all: clean test

tower:
	g++ -Wall tower.cpp -o tower

test: test1 test2 test3 test4 test5

test1: tower
	time ./tower < input1

test2: tower
	time ./tower < input2

test3: tower
	time ./tower < input3

test4: tower
	time ./tower < input4

test5: tower
	time ./tower < input5


mytest1: tower
	time ./tower < myinput1

clean:
	rm -rf tower


