#include <iostream>
#include <cassert>
#include <cstdbool>
#include <cmath>
#include <deque>
#include <vector>
#include <iomanip>

#define MAXN 210
#define MAXALL 1000000000
#define MAXPRICE 620
#define MAXDEFAULT 9200000000000000000

using namespace std;

typedef struct element {
    element* prev;
    long long int cost;
    int layer;
}Element;

typedef pair<int, int> pii;

void constructLpsTable(Element LPS[MAXPRICE][MAXN][MAXN], int A[], int cost[], int score[], int N, int D) {

    // Initialize tables
    // First table where cost = 0
    for(int i = 1; i <= N; ++i) {
        for(int j = 0; j <= N; ++j) {
            LPS[0][i][j].cost = 0;
            LPS[0][i][j].layer = 0;
            LPS[0][i][j].prev = NULL;
        }
    }

    // Tables where cost are in the range 1 ~ maxPrice
    int maxPrice = N * 3;

    for(int k = 1; k <= maxPrice; ++k) {
        for(int i = 1; i <= N; ++i) {
            for(int j = 1; j <= N; ++j) {
                LPS[k][i][j].cost = MAXDEFAULT;
                LPS[k][i][j].layer = k;
                LPS[k][i][j].prev = NULL;
            }
        }
    }

    for(int k = 1; k <= maxPrice; ++k) {

        // Set simple length strings if it's cost is as desired
        for(int i = 1; i <= N; ++i) {
            if(score[i] == k) {
                LPS[k][i][i].cost = 0;
                LPS[k][i][i].prev = NULL;
            }
        }
        // Variable used for storing values
        long long int leftCost, bottomCost, leftBottomCost, prevLeftBottomCost, minCost;

        // Constructing LPS table diagonally upwards rightwards
        for(int col = 2; col <= N; ++col) {


            for(int i = 1; i <= N - col + 1; ++i) {
                int j = i + col - 1;


                leftCost = LPS[k][i][j - 1].cost;
                bottomCost = LPS[k][i + 1][j].cost;
                leftBottomCost = LPS[k][i + 1][j - 1].cost;

                int prevLayer = k - score[i] - score[j];

                if(prevLayer >= 0) {
                    if(abs(A[i] - A[j]) <= D) {
                        prevLeftBottomCost = LPS[prevLayer][i + 1][j - 1].cost;
                    }
                    else {
                        if(cost[i] < cost[j]) {
                            prevLeftBottomCost = LPS[prevLayer][i + 1][j - 1].cost + cost[i];
                        }
                        else {
                            prevLeftBottomCost = LPS[prevLayer][i + 1][j - 1].cost + cost[j];
                        }
                    }
                }
                else
                    prevLeftBottomCost = MAXDEFAULT;

                // Special case where col = 2 and thier sum of value is equal to this layer
                if(col == 2) {
                    if(score[i] + score[j] == k) {
                        if(abs(A[i] - A[j]) <= D) {
                            LPS[k][i][j].cost = 0;
                        }
                        else {
                            if(cost[i] < cost[j])
                                LPS[k][i][j].cost = cost[i];
                            else
                                LPS[k][i][j].cost = cost[j];
                        }
                        LPS[k][i][j].prev = NULL;
                    }
                    minCost = min(leftCost, bottomCost);
                    if(LPS[k][i][j].cost > minCost) {
                        LPS[k][i][j].cost = minCost;
                        if(minCost == leftCost) {
                            LPS[k][i][j].prev = &LPS[k][i][j - 1];
                        }
                        else if(minCost == bottomCost) {
                            LPS[k][i][j].prev = &LPS[k][i + 1][j];
                        }
                    }
                    continue;
                }

                minCost = min(min(leftCost, bottomCost), min(leftBottomCost, prevLeftBottomCost));
                LPS[k][i][j].cost = minCost;

                // Assign costs
                if(minCost == leftCost) {
                    LPS[k][i][j].prev = &LPS[k][i][j - 1];
                }
                else if(minCost == bottomCost) {
                    LPS[k][i][j].prev = &LPS[k][i + 1][j];
                }
                else if(minCost == leftBottomCost){
                    LPS[k][i][j].prev = &LPS[k][i + 1][j - 1];
                }
                else if(minCost == prevLeftBottomCost) {
                    LPS[k][i][j].prev = &LPS[prevLayer][i + 1][j - 1];
                }
            }
        }
    }

#ifdef DEBUG
    // Print LPS table
    for(int k = 0; k <= maxPrice; ++k) {
        for(int i = 1; i <= N; ++i) {
            for(int j = 1; j <= N; ++j) {
                cout << setw(20) << LPS[k][i][j].cost << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
#endif

}

int main() {
    int N;  // The length of the sequences
    int K;  // The budget
    int D;  // The ADA sequence limit

    // Read in input
    cin >> N >> K >> D;

    int A[MAXN];  // The main sequence
    int cost[MAXN];  // The cost of changing A[i]
    int score[MAXN];  // The score of choosing A[i]
    static Element LPS[MAXPRICE][MAXN][MAXN];  // Main DP Table

    // Read in inputs
    for(int i = 1; i <= N; ++i)
        cin >> A[i];

    for(int i = 1; i <= N; ++i)
        cin >> cost[i];

    for(int i = 1; i <= N; ++i)
        cin >> score[i];

    // Construct LPS table
    constructLpsTable(LPS, A, cost, score, N, D);

    int bestScore = 0;

    for(int i = 3 * N; i > 0; --i) {
        if(LPS[i][1][N].cost <= K) {
            bestScore = i;
            break;
        }
    }
    // Print maximum possible score
    cout << bestScore << endl;


    Element* ptr = &LPS[bestScore][1][N];
    Element* start = &LPS[bestScore][1][1];

    // Record path by tracing pointers
    vector<pii> path;

    // Record operations when they happen
    vector<pii> operations;

    int row = (ptr - (start - 1)) / MAXN + 1, col =  (ptr - (start - 1)) % MAXN;

    path.push_back(make_pair(row, col));

    while(ptr -> prev != NULL) {

        if(ptr -> prev -> layer < ptr -> layer) {
            start = &LPS[ptr -> prev -> layer][1][1];
            if(abs(A[row] - A[col]) > D) {
                if(cost[row] < cost[col])
                    operations.push_back(make_pair(row, A[col]));
                else
                    operations.push_back(make_pair(col, A[row]));
            }
        }

        ptr = ptr -> prev;
        row = (ptr - start) / MAXN + 1;
        col = (ptr - start) % MAXN + 1;
        path.push_back(make_pair(row, col));
    }

    if(abs(A[row] - A[col]) > D) {
        if(cost[row] < cost[col])
            operations.push_back(make_pair(row, A[col]));
        else
            operations.push_back(make_pair(col, A[row]));
    }

#ifdef DEBUG
    // Print path
    for(int i = path.size() - 1; i >= 0; --i) {
        cout << "(" << path[i].first << " ," << path[i].second << ")" << endl;
    }
#endif


    // Print operations
    cout << operations.size() << endl;
    for(int i = 0; i < operations.size(); ++i) {
        cout << operations[i].first << " " << operations[i].second << endl;
    }

    vector<int> sequence;

    // Record ADA sequence
    for(int i = 0; i < path.size(); ++i)
        if(i == path.size() - 1 ||
        (path[i].first + 1 == path[i + 1].first && path[i].second - 1 == path[i + 1].second))
            sequence.push_back(path[i].first);

    // If middle is a pair
    if(path.back().first != path.back().second)
        sequence.push_back(path.back().second);

    for(int i = path.size() - 2; i >= 0; --i)
        if(path[i + 1].first - 1 == path[i].first && path[i + 1].second + 1 == path[i].second)
            sequence.push_back(path[i].second);

    cout << sequence.size() << endl;

    for(int i = 0; i < sequence.size(); ++i)
        cout << sequence[i] << " ";

    cout << endl;

    return 0;
}
