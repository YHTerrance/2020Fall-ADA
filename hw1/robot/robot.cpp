#include <iostream>
#include <vector>
#include <algorithm>

#define MAXR 510
#define MAXC 510

using namespace std;

typedef pair<int, int> pii;

typedef struct path{

    long long int totalSweetness;
    pii coords;
    struct path *prev;

}Path;

void makeFirstDpFarm(Path **DpFarm, int initialFarm[MAXR][MAXC], int rows, int cols) {

    // Set initial value

    for(int i = 0; i < rows; ++i)
        for(int j = 0; j < cols; ++j) {
            DpFarm[i][j].totalSweetness = initialFarm[i][j];
            DpFarm[i][j].coords = make_pair(i, j);
        }

    // deal with first row

    for(int j = 1; j < cols; ++j) {
        DpFarm[0][j].totalSweetness += DpFarm[0][j - 1].totalSweetness;
        DpFarm[0][j].prev = &DpFarm[0][j - 1];
    }

    // deal with first column

    for(int i = 1; i < rows; ++i) {
        DpFarm[i][0].totalSweetness += DpFarm[i - 1][0].totalSweetness;
        DpFarm[i][0].prev = &DpFarm[i - 1][0];
    }

    // deal with the rest

    for(int i = 1; i < rows; ++i) {
        for(int j = 1; j < cols; ++j) {

            if(DpFarm[i - 1][j].totalSweetness <= DpFarm[i][j - 1].totalSweetness) {
                DpFarm[i][j].totalSweetness += DpFarm[i][j - 1].totalSweetness;
                DpFarm[i][j].prev = &DpFarm[i][j - 1];
            }
            else {
                DpFarm[i][j].totalSweetness += DpFarm[i - 1][j].totalSweetness;
                DpFarm[i][j].prev = &DpFarm[i - 1][j];
            }

        }
    }

    return;
}

Path* findSweetest(Path** DpFarm, int rows, int cols) {
    Path* sweetest = &DpFarm[0][0];

    // Find sweetest in 2d farm

    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            if(sweetest -> totalSweetness <= DpFarm[i][j].totalSweetness)
                sweetest = &DpFarm[i][j];
        }
    }

    return sweetest;
}


void makeDpFarms(Path **DpFarm, Path **prevDpFarm, int initialFarm[MAXR][MAXC], int rows, int cols, int rocketsUsed, int decreaseConst) {

    // Set initial value

    for(int i = 0; i < rows; ++i)
        for(int j = 0; j < cols; ++j) {
            DpFarm[i][j].totalSweetness = (long long)initialFarm[i][j] - (long long)decreaseConst * (long long)rocketsUsed;
            DpFarm[i][j].coords = make_pair(i, j);
        }

    Path *prevSweetest = findSweetest(prevDpFarm, rows, cols);

    // deal with first element [0][0]

    DpFarm[0][0].totalSweetness += prevSweetest -> totalSweetness;
    DpFarm[0][0].prev = prevSweetest;

    // deal with first row

    for(int j = 1; j < cols; ++j) {

        if(DpFarm[0][j - 1].totalSweetness <= prevSweetest -> totalSweetness) {
            DpFarm[0][j].totalSweetness += prevSweetest -> totalSweetness;
            DpFarm[0][j].prev = prevSweetest;
        }
        else {
            DpFarm[0][j].totalSweetness += DpFarm[0][j - 1].totalSweetness;
            DpFarm[0][j].prev = &DpFarm[0][j - 1];
        }
    }

    // deal with first column

    for(int i = 1; i < rows; ++i) {

        if(DpFarm[i - 1][0].totalSweetness <= prevSweetest -> totalSweetness) {
            DpFarm[i][0].totalSweetness += prevSweetest -> totalSweetness;
            DpFarm[i][0].prev = prevSweetest;
        }
        else {
            DpFarm[i][0].totalSweetness += DpFarm[i - 1][0].totalSweetness;
            DpFarm[i][0].prev = &DpFarm[i - 1][0];
        }

    }

    // deal with the rest

    for(int i = 1; i < rows; ++i) {
        for(int j = 1; j < cols; ++j) {

            if(DpFarm[i - 1][j].totalSweetness <= DpFarm[i][j - 1].totalSweetness
               && prevSweetest -> totalSweetness <= DpFarm[i][j - 1].totalSweetness) {
                DpFarm[i][j].totalSweetness += DpFarm[i][j - 1].totalSweetness;
                DpFarm[i][j].prev = &DpFarm[i][j - 1];
            }
            else if(DpFarm[i][j - 1].totalSweetness <= DpFarm[i - 1][j].totalSweetness
               && prevSweetest -> totalSweetness <= DpFarm[i - 1][j].totalSweetness){
                DpFarm[i][j].totalSweetness += DpFarm[i - 1][j].totalSweetness;
                DpFarm[i][j].prev = &DpFarm[i - 1][j];
            }
            else {
                DpFarm[i][j].totalSweetness += prevSweetest -> totalSweetness;
                DpFarm[i][j].prev = prevSweetest;
            }
        }
    }

    return;
}

bool hasToJump(pii& from, pii& to) {

    if(to.first - from.first == 1 && to.second == from.second)
        return false;
    else if(to.first == from.first && to.second - from.second == 1)
        return false;
    else
        return true;

}


int main() {

    // Speed up input

    ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Read in input

    int rows, cols, rockets, decreaseConst;
    cin >> rows >> cols >> rockets >> decreaseConst;

    // Read in the sweetness of the initial farm

    int initialFarm[MAXR][MAXC];

    for(int i = 0; i < rows; ++i)
        for(int j = 0; j < cols; ++j)
            cin >> initialFarm[i][j];

    // Create 3d array

    Path ***DpFarms = new Path**[rockets + 1];

    for(int i = 0; i < rockets + 1; ++i) {
        DpFarms[i] = new Path*[rows];
        for(int j = 0; j < rows; ++j)
            DpFarms[i][j] = new Path[cols];
    }

    // Make the first dp farm without jumping

    makeFirstDpFarm(DpFarms[0], initialFarm, rows, cols);

    // Make the dp farms that have jumped

    for(int i = 1; i < rockets + 1; ++i)
        makeDpFarms(DpFarms[i], DpFarms[i - 1], initialFarm, rows, cols, i, decreaseConst);

    // Search for the best solution of all different jumps

    Path* best = &DpFarms[0][rows - 1][cols - 1];

    for(int i = 1; i < rockets + 1; ++i)
        if(best -> totalSweetness <= DpFarms[i][rows - 1][cols - 1].totalSweetness)
            best = &DpFarms[i][rows - 1][cols - 1];

    // Print sweetness and path length

    cout << best -> totalSweetness << endl;

    int length = 0;

    vector<pii> route;

    while(best -> prev != NULL) {
        route.push_back(best -> coords);
        best = best -> prev;
        length++;
    }

    // Add origin
    route.push_back(make_pair(0, 0));

    // Print length of path

    cout << length << endl;

    // Print the entire path

    for(int i = length - 1; i >= 0; --i) {

        if(hasToJump(route[i + 1], route[i]))
            cout << "Jump ";
        else
            cout << "Move ";

        cout << route[i].first << " " << route[i].second << endl;

    }

    // Free memory

    for(int i = 0; i < rockets + 1; ++i) {
        for(int j = 0; j < rows; ++j)
            delete[] DpFarms[i][j];
        delete[] DpFarms[i];
    }
    delete[] DpFarms;


    return 0;
}
