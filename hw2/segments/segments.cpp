#include <iostream>
#include <queue>
#include <algorithm>
#include <cassert>
#include <cstdbool>
#include <cstring>

using namespace std;

#define MAXN 200010
#define MAXM 200010

typedef struct segment_set {
    int left;
    int right;
    int id;
    long long int count;
}Segment_Set;

struct PQ_compare
{
    bool operator()(const Segment_Set& lhs, const Segment_Set& rhs) {
        if(lhs.right <= rhs.right)
            return true;
        else
            return false;
    }
};

int sets_compare_qsort (const void *a, const void *b) {
    Segment_Set* lhs = (Segment_Set*) a;
    Segment_Set* rhs = (Segment_Set*) b;

    if(lhs -> left <= rhs -> left)
        return false;
    else
        return true;
}

int sets_compare(const Segment_Set& lhs, const Segment_Set& rhs) {
    if(lhs.left < rhs.left)
        return 1;
    else
        return -1;
}


bool canCoverRequirement(Segment_Set sets[], long long int selected[MAXN], long long int requirement, int N, int M, long long int K) {

    priority_queue<Segment_Set, vector<Segment_Set>, PQ_compare> PQ;

    // Current thickness
    long long int thickness = 0;

    long long int area[MAXM] = {};

    // Selected amount for each segment set
    long long int selectedAmount;
    long long int totalSelected = 0;

    // Current index for sets[]
    int currSets = 0;

    area[1] = -requirement;

    // Store the segment set popped from PQ
    Segment_Set current;

    for(int i = 1; i <= M; ++i) {

        // Add usable segment sets to PQ
        while(currSets < N && sets[currSets].left < i) {
            PQ.push(sets[currSets]);
            #ifdef DEBUG
            cout << "Pushing " << sets[currSets].left << " " << sets[currSets].right << endl;
            #endif
            ++currSets;
        }

        // Calculate thickness at current point
        thickness += area[i];

        #ifdef DEBUG
        // Print area
        cout << "================" << endl;
        for(int i = 1; i <= M; ++i) {
            cout << area[i] << " ";
        }
        cout << endl;
        cout << "================" << endl;

        #endif

        // When the thickness is in deficit
        while(thickness < 0) {

            #ifdef DEBUG
            cout << "thickness: " << thickness << endl;
            #endif

            // if PQ is empty
            if(PQ.empty()) {
                #ifdef DEBUG
                cerr << "EMPTY PQ!!" << endl << endl << endl;
                #endif
                return false;
            }

            current = PQ.top();
            PQ.pop();

            // If segment set not in range keep popping
            if(current.right < i) {
                #ifdef DEBUG
                cerr << "No more valid segment sets" << endl << endl << endl;
                #endif

                return false;
            }

            // If this current segment set can cover our deficit
            if(current.count + thickness > 0) {

                selectedAmount = -thickness;

                // Thickness < 0
                current.count -= selectedAmount;

                PQ.push(current);

                thickness = 0;
            }
            // If it can only do so much
            else {

                selectedAmount = current.count;

                // Current segment set is completely used up
                thickness += selectedAmount;
            }

            #ifdef DEBUG
            cout << "id: " << current.id << " selected: " << selectedAmount << endl;
            #endif

            totalSelected += selectedAmount;

            // Exceed budget limit
            if(totalSelected > K) {
                #ifdef DEBUG
                cerr << "Exceed budget" << endl << endl << endl;
                #endif
                return false;
            }

            selected[current.id] += selectedAmount;
            area[current.right + 1] -= selectedAmount;

        }

    }

    #ifdef DEBUG
    cout << "Done!!!" << endl << endl;
    #endif

    return true;
}

// Segment_Set sets[MAXN];
Segment_Set sets[MAXN];

int main() {
    // N: number of segment sets
    // M: Range of the segments
    // K: Segments you can select
    long long int N, M, K;
    cin >> N >> M >> K;

    // Read in segment sets
    for(int i = 0; i < N; ++i) {
        cin >> sets[i].left >> sets[i].right >> sets[i].count;
        sets[i].id = i + 1;
    }

    qsort(sets, N, sizeof(Segment_Set), sets_compare_qsort);

    #ifdef DEBUG
    for(int i = 0; i < N; ++i) {
        cout << sets[i].left << " " << sets[i].right << " " << sets[i].count << endl;
    }
    #endif

    // sort(sets, sets + N, sets_compare);

    // The amount selected of respective segment sets
    long long int selected[MAXN] = {};

    long long int lowerBound = 0, upperBound = K;

    while(lowerBound < upperBound) {
        long long int middle = lowerBound + (upperBound - lowerBound) / 2;

        // Check if the requirement can be fullfilled
        if (canCoverRequirement(sets, selected, middle, N, M, K))
            lowerBound = middle + 1;

        else
            upperBound = middle - 1;

    }

    // Reset selected
    memset(selected, 0, (N + 1) * sizeof(long long int));

    if(canCoverRequirement(sets, selected, lowerBound, N, M, K)) {
        #ifdef DEBUG
        cout << "First case" << endl;
        #endif

        cout << lowerBound << endl;
    }
    else {

        #ifdef DEBUG
        cout << "Second case" << endl;
        #endif

        memset(selected, 0, (N + 1) * sizeof(long long int));
        canCoverRequirement(sets, selected, lowerBound - 1, N, M, K);
        cout << lowerBound - 1 << endl;
    }

    // Print final selected
    for(int i = 1; i <= N; ++i)
        cout << selected[i] << " ";

    cout << endl;

    return 0;
}
