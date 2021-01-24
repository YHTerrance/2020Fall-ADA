all: test

PHONY:
	boooks

boooks:
	g++ -Wall boooks.cpp -o boooks

test: test1 test2 test3 test4

test1: boooks
	time ./boooks < input1

test2: boooks
	time ./boooks < input2

test3: boooks
	time ./boooks < input3

test4: boooks
	time ./boooks < input4

mytest1: boooks
	time ./boooks < myinput1

clean:
	rm -rf boooks


