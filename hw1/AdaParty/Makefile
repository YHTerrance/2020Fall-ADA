all: test

party:
	g++ -Wall party.cpp -o party
assert:
	g++ -Wall party.cpp -o party -DASSERT


.PHONY: party


test: test1 test2 mytest1

test1: party
	time ./party < input1

test2: party
	time ./party < input2

test3: party
	time ./party < input3

test4: party
	time ./party < input4

mytest1: party
	time ./party < myinput1

clean:
	rm -rf party


