all: test

party:
	g++ -std=c++11 -Wall party.cpp -o party

debug:
	g++ -std=c++11 -Wall party.cpp -DDEBUG -o party

test: test1 test2 test3 test4

test1: party
	time ./party < input1

test2: party
	time ./party < input2

test3: party
	time ./party < input3

mytest1: party
	time ./party < myinput1

mytest2: party
	time ./party < myinput2

clean:
	rm -rf party


