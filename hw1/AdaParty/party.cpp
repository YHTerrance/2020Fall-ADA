#include <iostream>
#include <algorithm>
#include <cstring>
#include <cassert>
#include <climits>

#define MAXN 500000
#define MAXK 500000
#define MAXA 1000000000

using namespace std;

static int A[MAXK + 1] = {}, B[MAXK + 1] = {}, C[MAXK + 1] = {};

static int bags[MAXN + 1]; // Number of candies in bags
static long long int prefixSum[MAXN + 1]; // Prefix sum of bags[]
static int minOfRight[MAXN + 1]; // Min value for every right interval
static int maxOfRight[MAXN + 1]; // Max value for every right interval

void clearABC(int rMin, int rMax, int start, int end, int k) {

    // Shared variable
    long long int sum;

    int iter = start;
    if(rMax >= rMin) {
        while(iter < rMin) {
            sum = prefixSum[iter] - prefixSum[start - 1];
            --A[sum % k];

            ++iter;
        }

        while(iter < rMax) {
            sum = prefixSum[iter] - prefixSum[start - 1] - minOfRight[iter];
            --B[sum % k];

            ++iter;
        }
    }
    else {
        while(iter < rMax) {
            sum = prefixSum[iter] - prefixSum[start - 1];
            --A[sum % k];

            ++iter;
        }

        while(iter < rMin) {
            sum = prefixSum[iter] - prefixSum[start - 1] - maxOfRight[iter];
            --B[sum % k];

            ++iter;
        }
    }

    while(iter <= end) {
        sum = prefixSum[iter] - prefixSum[start - 1] - minOfRight[iter] - maxOfRight[iter];
        --C[sum % k];

        ++iter;
    }



    return;
}


long long int findPairs(int bags[], long long int prefixSum[], int left, int right, int k) {

    // Base case
    if(left == right)
        return 0;
    else if(right - left == 1)
        return 1;


    long long int count = 0;
    long long int sum; // Shared variable

    int mid = (left + right) / 2;

    minOfRight[mid] = MAXA + 1;
    maxOfRight[mid] = -1;

    // Initialize C[]
    for(int i = mid + 1; i <= right; ++i) {

        if(bags[i] < minOfRight[i - 1])
            minOfRight[i] = bags[i];
        else
            minOfRight[i] = minOfRight[i - 1];

        if(bags[i] > maxOfRight[i - 1])
            maxOfRight[i] = bags[i];
        else
            maxOfRight[i] = maxOfRight[i - 1];

        if(i == mid + 1)
            continue;

        sum = prefixSum[i] - prefixSum[mid] - maxOfRight[i] - minOfRight[i];
        ++C[sum % k];
    }

    // All signify indexes
    int lCur = mid, lMax = mid, lMin = mid, rMin = mid + 1, rMax = mid + 1;

    // Deal with bags[mid + 1] (must belong to B[] or A[])
    if(bags[lMax] >= bags[rMax])
        ++rMax;
    if(bags[lMin] <= bags[rMin])
        ++rMin;
    if(rMax < rMin) {
        sum = prefixSum[mid + 1] - prefixSum[mid] - maxOfRight[mid + 1];
        ++B[sum % k];
    }
    else if(rMax > rMin) {
        sum = prefixSum[mid + 1] - prefixSum[mid] - minOfRight[mid + 1];
        ++B[sum % k];
    }
    else {
        sum = prefixSum[mid + 1] - prefixSum[mid];
        ++A[sum % k];
    }

    while(lCur >= left) {

        // Update lMin, lMax
        if(bags[lCur] <= bags[lMin])
            lMin = lCur;
        if(bags[lCur] > bags[lMax])
            lMax = lCur;

        // Move the rMin pointer
        while(rMin <= right && bags[lMin] <= bags[rMin]) {

            // rMin at the right of rMax
            if(rMin >= rMax) {
                // Take from C[]
                sum = prefixSum[rMin] - prefixSum[mid] - maxOfRight[rMin] - minOfRight[rMin];
                --C[sum % k];

                sum += minOfRight[rMin];
                ++B[sum % k];
            }
            else {
                // Take from B[]
                sum = prefixSum[rMin] - prefixSum[mid] - minOfRight[rMin];
                --B[sum % k];

                sum += minOfRight[rMin];
                ++A[sum % k];
            }
            ++rMin;
        }
        // Move the rMax pointer
        while(rMax <= right && bags[lMax] >= bags[rMax]) {
            if(rMax >= rMin) {
                // Take from C[]
                sum = prefixSum[rMax] - prefixSum[mid] - maxOfRight[rMax] - minOfRight[rMax];
                --C[sum % k];

                sum += maxOfRight[rMax];
                ++B[sum % k];
            }
            else {
                // Take from B[]
                sum = prefixSum[rMax] - prefixSum[mid] - maxOfRight[rMax];
                --B[sum % k];

                sum += maxOfRight[rMax];
                ++A[sum % k];
            }
            ++rMax;
        }

        // Finish moving pointers, start counting pairs
        long long int leftSum = prefixSum[mid] - prefixSum[lCur - 1];

        // Case A
        int remainder;

        remainder = (leftSum - bags[lMin] - bags[lMax]) % k;

        if(remainder == 0)
            count += A[0];
        else if(remainder < 0)
            count += A[-remainder];
        else
            count += A[k - remainder];

        // Case B
        if(rMin <= rMax)
            remainder = (leftSum - bags[lMax]) % k;
        else
            remainder = (leftSum - bags[lMin]) % k;

        if(remainder == 0)
            count += B[0];
        else
            count += B[k - remainder];

        // Case C
        remainder = leftSum % k;

        if(remainder == 0)
            count += C[0];
        else
            count += C[k - remainder];

        --lCur;
    }
#ifdef ASSERT
    for(int i = left; i <= right; ++i)
        cout << bags[i] << " ";
    cout << "count = " << count << endl;
#endif
    // Remove everything from A, B, C
    clearABC(rMin, rMax, mid + 1, right, k);

    return count + findPairs(bags, prefixSum, left, mid, k) + findPairs(bags, prefixSum, mid + 1, right, k);
}

int main() {

    // Faster input
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int numOfBags, numOfClassmates; // Total number of bags and classmates (excluding YP, BB)
    cin >> numOfBags >> numOfClassmates;

    // Set padding value
    prefixSum[0] = bags[0] = 0;

    for(int i = 1; i <= numOfBags; ++i)
        cin >> bags[i];

    for(int i = 1; i <= numOfBags; ++i)
        prefixSum[i] = prefixSum[i - 1] + bags[i];

    cout << findPairs(bags, prefixSum, 1, numOfBags, numOfClassmates) << endl;

    return 0;
}
