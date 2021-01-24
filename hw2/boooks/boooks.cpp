#include <iostream>
#include <cassert>
#include <cstdbool>
#include <climits>

#define MAXN 100010

using namespace std;

int main() {
    // Faster input
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    // Read in the range of the thickness of books
    cin >> n;

    // Array that stores the number of books for every different thickness
    int original[MAXN] = {};
    int added[MAXN] = {};
    int added2[MAXN] = {};
    int tmp[MAXN] = {};
    int type = 1;

    // The index of the first non-zero element
    int start = -1, end = -1;

    for(int i = 1; i <= n; ++i) {
        cin >> original[i];
        added[i] = added2[i] = original[i];
    }

    for(int i = 1; i <= n; ++i)
        if(original[i] > 0) {
            start = i;
            break;
        }

    for(int i = n; i >= 1; --i)
        if(original[i] > 0) {
            end = i;
            break;
        }

    long long int booksAdded = 0, booksAdded2 = LLONG_MAX;

    // Only one is non-zero
    if(start == end) {
        if(start > 1) {
            added[start - 1] += original[start] - 1;
        }
        else {
            added[start + 1] += original[start] - 1;

            if(added[start + 1] > 0)
                // Move end back
                ++end;
        }

        booksAdded += original[start] - 1;
    }

    // Only two are non-zero
    else if(end - start == 1) {

        if(original[start] + 1 < original[start + 1]) {
            booksAdded += original[start + 1] - original[start] - 1;
            added[start] += original[start + 1] - original[start] - 1;
        }
        else if(original[start] > original[start + 1] + 1) {
            booksAdded = original[start] - original[start + 1] - 1;
            added[start + 1] += original[start] - original[start + 1] - 1;
        }

        // Change starting point to second
        if(added[start + 1] > added[start])
            ++start;

    }

    // Spanning more than three terms
    else if(end - start > 1) {


        int last_term = original[end];

        // Create tmp array
        for(int i = start; i <= end; ++i)
            tmp[i] = original[i];


        for(int i = start; i <= end; ++i) {
            // At least need one
            --tmp[i];

            // Pay debt
            if(tmp[i - 1] > 0) {
                tmp[i] -= tmp[i - 1];
            }

            if(tmp[i] < 0) {

                booksAdded += -tmp[i];
                added[i] += -tmp[i];
                tmp[i] = 0;
            }
        }

        // Reached end
        if(tmp[end] > 0) {
            booksAdded += tmp[end];
            added[end - 1] += tmp[end];
            tmp[end] = 0;
        }

        // Last term can be omitted
        if(added[end] - last_term > 0) {
            --added[end];
            --booksAdded;
        }

        // Calculate second type

        booksAdded2 = 0;
        // Create tmp array
        for(int i = start; i <= end; ++i)
            tmp[i] = original[i];

        for(int i = start + 1; i <= end; ++i) {
            // At least need one
            --tmp[i];

            // Pay debt
            if(tmp[i - 1] > 0) {
                tmp[i] -= tmp[i - 1];
            }

            if(tmp[i] < 0) {

                booksAdded2 += -tmp[i];
                added2[i] += -tmp[i];
                tmp[i] = 0;
            }
        }

        // Reached end
        if(tmp[end] > 0) {
            booksAdded2 += tmp[end];
            added2[end - 1] += tmp[end];
            tmp[end] = 0;
        }

        // Last term can be omitted
        if(added2[end] - last_term > 0) {
            --added2[end];
            --booksAdded2;
        }

    }

    if(booksAdded2 < booksAdded)
        type = 2;

    // cout << "start = " << start << " end = " << end << endl;

    // for(int i = 1; i <= n; ++i) {
    //     cout << "added " << i << " = " << added[i] << endl;
    // }

    if(type == 2)
        cout << booksAdded2 << endl;
    else
        cout << booksAdded << endl;

    bool flag;
    cin >> flag;
    // Flag that specifies if we should output entire result
    if(flag) {
        if(type == 1) {
            for(int i = start; i <= end; ++i) {

                cout << i << " ";
                --added[i];

                while(added[i - 1]) {

                    cout << i - 1 << " ";
                    --added[i - 1];

                    if(added[i]) {
                        cout << i << " ";
                        --added[i];
                    }
                    else {
                        assert(added[i - 1] <= 0);
                    }

                }
                assert(added[i - 1] == 0);
            }
            assert(added[end] == 0);
        }
        else {
            for(int i = start + 1; i <= end; ++i) {

                cout << i << " ";
                --added2[i];

                while(added2[i - 1]) {

                    cout << i - 1 << " ";
                    --added2[i - 1];

                    if(added2[i]) {
                        cout << i << " ";
                        --added2[i];
                    }
                    else {
                        assert(added2[i - 1] <= 0);
                    }

                }
                assert(added2[i - 1] == 0);
            }
            assert(added2[end] == 0);
        }
    }

    return 0;
}
