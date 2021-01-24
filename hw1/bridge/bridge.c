#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>

#define MAXN 200000

#define MAXC 1000000000



// Solution consultance: https://www.geeksforgeeks.org/closest-pair-of-points-using-divide-and-conquer-algorithm/

// A structure to store a point in the 2D plane

typedef struct point {
    long long int x, y;
}Point;

Point pX[MAXN], pY[MAXN];

// Sort array according to X coordinates

int compareX(const void *a, const void *b) {
    Point *p1 = (Point *)a, *p2 = (Point *)b;

    if(p1 -> x > p2 -> x)
        return 1;
    else if(p1 -> x < p2 -> x)
        return -1;
    else if(p1 -> y > p2 -> y)
        return 1;
    else if(p1 -> y < p2 -> y)
        return -1;
    else
        return 0;
}

// Sort array according to Y coordinates

int compareY(const void *a, const void *b) {
    Point *p1 = (Point *)a, *p2 = (Point *)b;
    if(p1 -> y > p2 -> y)
        return 1;
    else if(p1 -> y < p2 -> y)
        return -1;
    else if(p1 -> x > p2 -> x)
        return 1;
    else if(p1 -> x < p2 -> x)
        return -1;
    else
        return 0;
}

// Calculate the square of the Euclidean distance between two points

long long int distance(Point *p1, Point *p2) {
    long long int distance = ((p1 -> x - p2 -> x) * (p1 -> x - p2 -> x)) +
           ((p1 -> y - p2 -> y) * (p1 -> y - p2 -> y));
    if(distance == 0) {
        printf("0\n");
        exit(0);
    }
    return distance;
}

// Return the smaller of two integers

long long int min(long long int x, long long int y) {
    return (x > y) ? y : x;
}

// Find smallest distance within strip

long long int findClosestDistanceInStrip(Point middleStrip[], int n, long long int d) {
    long long int min = d;

    // Search for point pairs that have smaller distance than d

    for(int i = 0; i < n; i++) {
        for(int j = i + 1; j < n && (middleStrip[j].y - middleStrip[i].y) * (middleStrip[j].y - middleStrip[i].y) < min; j++) {
            long long int dist = distance(&middleStrip[i], &middleStrip[j]);
            if(dist < min)
                min = dist;
        }
    }

    return min;
}


// Find smallest distance recursively

long long int findClosestDistance(Point pX[], Point pY[], int n) {
    if(n == 2)
        return distance(&pX[0], &pX[1]);
    else if(n == 3)
        return min(min(distance(&pX[0], &pX[1]), distance(&pX[1], &pX[2])), distance(&pX[0], &pX[2]));

    // Find the middle point

    int mid = n / 2;

    Point midPoint = pX[mid];

    // Divide points in y sorted array around the vertical line.

    Point pYl[mid];   // y sorted points on left of vertical line
    Point pYr[n - mid];  // y sorted points on right of vertical line
    bool foundMid = false; // set flag for sorting left and right
    int li = 0, ri = 0;  // indexes of left and right subarrays

    for(int i = 0; i < n; i++) {
        if(pY[i].x < midPoint.x)
            pYl[li++] = pY[i];
        else if(pY[i].x > midPoint.x)
            pYr[ri++] = pY[i];
        else if(pY[i].y == midPoint.y || foundMid) {
            foundMid = true;
            pYr[ri++] = pY[i];
        }
        else
            pYl[li++] = pY[i];
    }

    // Calculate the minimum distance dl on the left and dr on the right of the vertical line that passes the middle point

    long long int dl = findClosestDistance(pX, pYl, mid);
    long long int dr = findClosestDistance(pX + mid, pYr, n - mid);

    // Record the smaller of the two

    long long int d = min(dl, dr);

    // Construct an array middleStrip[] that contains points closer than d to the vertical line that passes through the middle point

    Point middleStrip[n];

    int cnt = 0;

    for(int i = 0; i < n; i++)
        if((pY[i].x - midPoint.x) * (pY[i].x - midPoint.x) < d)
            middleStrip[cnt++] = pY[i];

    // Return the minimum of d and the closest distance within the strip
    return min(d, findClosestDistanceInStrip(middleStrip, cnt, d));
}


int main() {

    // Number of candidates

    int n;
    scanf("%d", &n);

    //Point *pX = (Point *)(malloc(MAXN * sizeof(Point))), *pY = (Point *)(malloc(MAXN * sizeof(Point)));

    for(int i = 0; i < n; i++) {
        scanf("%lld %lld", &pX[i].x, &pX[i].y);
        pY[i].x = pX[i].x; pY[i].y = pX[i].y;
    }

    // Sort array according to X coordinates

    qsort(pX, n, sizeof(Point), compareX);
    qsort(pY, n, sizeof(Point), compareY);

    printf("%lld\n", -findClosestDistance(pX, pY, n));
    return 0;
}
