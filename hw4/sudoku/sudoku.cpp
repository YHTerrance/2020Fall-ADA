#include <iostream>
#include <unordered_map>
#include <string>
#include <set>
#include <cassert>
#include <cstdbool>
#include <vector>
#include <algorithm>

using namespace std;

#define FOR(i, way, to) for(int i = way[to] ; i != to ; i = way[i])

namespace DLX {
    int col_size, sz;
    vector<int> s, l, r, u, d, row, col, current;
    void Init(int n) {
        s.clear();
        l.clear();
        r.clear();
        u.clear();
        d.clear();
        row.clear();
        col.clear();
        current.clear();
        col_size = n;
        for (int i = 0; i <= n; ++ i) {
            u.emplace_back(i);
            d.emplace_back(i);
            l.emplace_back(i - 1);
            r.emplace_back(i + 1);
        }
        s.resize(n + 1, 0);
        row.resize(n + 1, 0);
        col.resize(n + 1, 0);
        r[n] = 0, l[0] = n, sz = n + 1;
    }
    void AddRow(int rr, vector<int> &sol) {
        int tmp = sz;
        for (auto to : sol) {
            l.emplace_back(sz - 1);
            r.emplace_back(sz + 1);
            d.emplace_back(to);
            u.emplace_back(u[to]);
            d[u[to]] = sz, u[to] = sz;
            row.emplace_back(rr);
            col.emplace_back(to);
            s[to] ++, sz ++;
        }
        r[sz - 1] = tmp, l[tmp] = sz - 1;
    }
    void Remove(int c) {
        l[r[c]] = l[c];
        r[l[c]] = r[c];
        FOR(i, d, c) FOR(j, r, i) {
            u[d[j]] = u[j];
            d[u[j]] = d[j];
            --s[col[j]];
        }
    }
    void Restore(int c) {
        FOR(i, u, c) FOR(j, l, i) {
            ++s[col[j]], u[d[j]] = d[u[j]] = j;
        }
        l[r[c]] = r[l[c]] = c;
    }
    void Dfs(int floor) {
        if(r[0] == 0) return;
        int c = r[0];
        FOR(i, r, 0) {
            if (s[i] <= s[c]) c = i;
            if (s[i] == 0) return;
        }
        Remove(c);
        FOR(i, d, c) {
            FOR(j, r, i) Remove(col[j]);
            current.emplace_back(row[i]);
            Dfs(floor + 1);
            if (r[0] == 0) return;
            current.pop_back();
            FOR(j, l, i) Restore(col[j]);
        }
        Restore(c);
    }
    vector<int> Solver() {
        return Dfs(0), current;
    }
}

#undef FOR

typedef pair<int, int> pii;

void findCombinationUtil(int arr[], int index, int num, int reducedNum, vector< vector<int> >& colorCombinations, int colorCount) {
    // Base condition
    if(reducedNum < 0 || index > colorCount)
        return;

    // If combination is found store it
    if(reducedNum == 0) {
        if(index == colorCount) {
            vector<int> tmp;
            for(int i = 0; i < index; ++i)
                tmp.push_back(arr[i]);
            // Store all possible combinations
            colorCombinations.push_back(tmp);
        }
        return;
    }

    int prev = (index == 0) ? 1 : arr[index - 1] + 1;

    for(int k = prev; k <= num && k <= 9; ++k) {
        arr[index] = k;
        findCombinationUtil(arr, index + 1, num, reducedNum - k, colorCombinations, colorCount);
    }
}

void findCombinations(vector< vector<int> >& colorCombinations, int colorNumber, int colorCount) {
    int arr[colorCount];
    findCombinationUtil(arr, 0, colorNumber, colorNumber, colorCombinations, colorCount);
}

void addNormalSudoku(vector< vector<int> > &SudokuDLX, int i, int row, int col, int value) {

  // Position constraint: only 1 number can occupy a cell
  SudokuDLX[i].push_back(i / 9 + 1);

  // Row constraint: only 1 instance of a number can be in a row
  SudokuDLX[i].push_back(81 + value + 1 + row * 9);

  // Column constraint: only 1 instance of a number can be in a column
  SudokuDLX[i].push_back(162 + i % 81 + 1);

  // Region constraint: only 1 instance of a number can be in a region
  // Region 1
  if(row < 3 && col < 3)
      SudokuDLX[i].push_back(243 + value + 1);
  // Region 2
  else if(row < 3 && col < 6)
      SudokuDLX[i].push_back(243 + 9 + value + 1);
  // Region 3
  else if(row < 3 && col < 9)
      SudokuDLX[i].push_back(243 + 18 + value + 1);
  // Region 4
  else if(row < 6 && col < 3)
      SudokuDLX[i].push_back(243 + 27 + value + 1);
  // Region 5
  else if(row < 6 && col < 6)
      SudokuDLX[i].push_back(243 + 36 + value + 1);
  // Region 6
  else if(row < 6 && col < 9)
      SudokuDLX[i].push_back(243 + 45 + value + 1);
  // Region 7
  else if(row < 9 && col < 3)
      SudokuDLX[i].push_back(243 + 54 + value + 1);
  // Region 8
  else if(row < 9 && col < 6)
      SudokuDLX[i].push_back(243 + 63 + value + 1);
  // Region 9
  else if(row < 9 && col < 9)
      SudokuDLX[i].push_back(243 + 72 + value + 1);
  else
      assert(false);
}




int main() {
  unordered_map< char, vector< vector<int> > > allCombinations;
  unordered_map< char, vector<pii> > colorRegions;
  unordered_map< char, int > colorNumbers;

  set<char> colors;
  string sudoku[9];

  // Read in sudoku board
  for(int i = 0; i < 9; ++i)
      cin >> sudoku[i];

  // Store color info in unordered_map
  for(int i = 0; i < 9; ++i) {
      for(int j = 0; j < 9; ++j) {
          char color = sudoku[i][j];
          colors.insert(color);
          colorRegions[color].push_back(make_pair(i ,j));
      }
  }

  // Read in the number color info
  for(int i = 0; i < colors.size(); ++i) {
      char color;
      int number;
      cin >> color >> number;
      colorNumbers[color] = number;
  }

  // Store the combination info of all colors
  for(auto color : colors) {
      #ifdef DEBUG
      printf("%c: %d\n", color, colorNumbers[color]);
      #endif

      findCombinations(allCombinations[color], colorNumbers[color], colorRegions[color].size());

      #ifdef DEBUG
      for(auto i : allCombinations[color]) {
          for(auto j : i)
              cout << j << " ";
          cout << endl;
      }
      #endif
  }
  vector< vector<int> > SudokuDLX;

  SudokuDLX.resize(729);

  int currentCol = 325;

  for(auto color : colors) {

    // Values that are not used
    bool unused[10];
    for(int i = 1; i <= 9; ++i)
      unused[i] = true;

    for(auto combination : allCombinations[color]) {
      for(auto ele : combination) {
        if(unused[ele]) {
          unused[ele] = false;
        }
      }
    }

    int row, col, index, addedCols;

    // iterate through all boxes of the region
    for(int i = 0; i < colorRegions[color].size(); ++i) {
      addedCols = 0;
      row = colorRegions[color][i].first;
      col = colorRegions[color][i].second;
      index = row * 81 + col * 9;

      // Try to add cols
      for(int j = 0; j < 9; ++j) {
        if(!unused[j + 1]) {
          addNormalSudoku(SudokuDLX, index + j, row, col, j);
          SudokuDLX[index + j].push_back(currentCol + addedCols);
          ++addedCols;
        }
      }
    }

    // Try to add new row if it has more than one combination
    if(allCombinations[color].size() > 1) {
      for(auto combination : allCombinations[color]) {
        auto it = combination.begin();
        vector<int> newRow;
        addedCols = 0;

        for(int j = 0; j < 9; ++j) {
          if(!unused[j + 1]) {
            if(it != combination.end() && (*it - 1) == j)
              ++it;
            else
              newRow.push_back(currentCol + addedCols);
            ++addedCols;
          }
        }
        SudokuDLX.push_back(newRow);
      }
    }
    currentCol += addedCols;
  }

  #ifdef DEBUG
  cout << "Sudoku board:" << endl;
  for(auto row : SudokuDLX) {
      if(!row.empty()) {
        for(auto col : row) {
          cout << col << " ";
      }
      cout << endl;
      }
  }
  #endif

  int addedRows = 0;
  int totalCols = currentCol - 1;

  DLX::Init(totalCols);

  // Add to DLX
  for(int i = 0; i < SudokuDLX.size(); ++i) {
    if(!SudokuDLX[i].empty()) {
      ++addedRows;
      DLX::AddRow(i, SudokuDLX[i]);
    }
  }

  vector<int> sol = DLX::Solver();

  #ifdef DEBUG
  cout << "Sudoku rows: " << addedRows << " cols: " << totalCols << endl;

  for(auto index : sol)
    cout << index << " ";
  cout << endl;
  #endif

  for(auto index : sol)
      if(index < 729)
          sudoku[index / 81][(index % 81) / 9] = '1' + (index % 9);

  for(int i = 0; i < 9; ++i)
      cout << sudoku[i] << endl;

  return 0;
}

