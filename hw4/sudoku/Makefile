sudoku: sudoku.cpp
	g++ -std=c++11 -Wall sudoku.cpp -o sudoku

debug:
	g++ -std=c++11 -Wall sudoku.cpp -DDEBUG -o sudoku

test1: sudoku
	time ./sudoku < input1

clean:
	rm -rf sudoku


