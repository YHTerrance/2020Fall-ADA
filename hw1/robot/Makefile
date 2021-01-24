all: test

robot:
	g++ -Wall robot.cpp -o robot

test: test1 test2 mytest1

test1: robot
	time ./robot < input1

test2: robot
	time ./robot < input2

test3: robot
	time ./robot < input3

mytest1: robot
	time ./robot < myinput1

clean:
	rm -rf robot


