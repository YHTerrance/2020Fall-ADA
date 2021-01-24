#include <iostream>
#include <algorithm>
#include <climits>
#include <cassert>

#define MAXE 100000
#define MAXSIZE 100000

/*
  Helper header file for HW1 Problem B, NTU CSIE ADA, Fall 2020

  Author: ADA TA

  Usage:
      This header file provides an one dimensional memory that
      supports O(log(N)) time range addition operation. You can
      simply include this header file, or copy and paste this
      snippet on top of your source code. Feel free to modify
      this to meet your needs.
*/

#ifndef _HELPER_H
#define _HELPER_H

namespace Memory {
    constexpr int N = 100000; // Number of cells, change this if you need more cells

    long long arr[N + 1]; // Note: This is NOT the memory itself, please don't try to access
                          //       this array directly unless you know what you're doing

    /* Start of internal functions */
    void _ins(int x, int k) {
        while (x <= N) arr[x] += k, x += x & -x;
    }

    long long _qr(int x) {
        long long ret = 0;
        while (x) ret += arr[x], x -= x & -x;
        return ret;
    }
    /* End of internal functions */

    // Explicitly fill the whole memory with zero
    // Complexity: O(N)
    void reset() {
        for (int i = N; i >= 0; --i)
            arr[i] = 0;
    }

    // Add k to every cell in range [l, r]
    // Complexity: O(log(N))
    void add(int l, int r, int k) {
        assert(1 <= l && l <= r && r <= N && "add: the argument should satisfy 1 <= l <= r <= N");
        _ins(l, k);
        _ins(r + 1, -k);
    }

    // Get the value in cell x
    // Complexity: O(log(N))
    long long get(int x) {
        assert(1 <= x && x <= N && "get: x should be a positive integer in range [1, N]");
        return _qr(x);
    }
}
#endif // _HELPER_H


using namespace std;

// Define event struct
typedef struct event {

    char type;
    int location;
    int value;
    long long int damage;
    int left;
    int right;

} Event;

void swapEventPtr(Event **ptr1, Event **ptr2) {
    Event *tmp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = tmp;
    return;
}

bool withinRadius(Event *player, Event *bomb) {
    assert(player -> type == 'P' && bomb -> type == 'A');

    return (player -> location <= bomb -> right && player -> location >= bomb -> left);
}

void bruteForce(Event *bombEventPtr[], int left, int right) {

    // Three events
    if(right - left == 2) {
        Event *first = bombEventPtr[left], *second = bombEventPtr[left + 1], *third = bombEventPtr[left + 2];

        // P A x or P x A
        if(first -> type == 'P') {
            if(second -> type == 'A')
                // Player is within radius
                if(withinRadius(first, second))
                    // Player cannot defend
                    if(first -> value <= second -> value)
                        first -> damage += second -> damage;

            if(third -> type == 'A')
                // Player is within radius
                if(withinRadius(first, third))
                    // Player cannot defend
                    if(first -> value <= third -> value)
                        first -> damage += third -> damage;
        }

        // x P A
        if(second -> type == 'P') {
            if(third -> type == 'A')
                // Player is within radius
                if(withinRadius(second, third))
                    // Player cannot defend
                    if(second -> value <= third -> value)
                        second -> damage += third -> damage;
        }
    }

    // Two events
    else if(right - left == 1) {
        Event *first = bombEventPtr[left], *second = bombEventPtr[left + 1];

        // P A
        if(first -> type == 'P')
            if(second -> type == 'A')
                // Player is within radius
                if(withinRadius(first, second))
                    // Player cannot defend
                    if(first -> value <= second -> value)
                        first -> damage += second -> damage;
    }
    else
        cout << "Error: right == left, divided till only one" << endl;

    // bubble sort according to values
    for(int i = 0; i < right - left; ++i)
        for(int j = left; j < right - i; ++j)
            if(bombEventPtr[j] -> value > bombEventPtr[j + 1] -> value)
                swapEventPtr(&bombEventPtr[j], &bombEventPtr[j + 1]);

    return;
}

// Merge two runned events

void merge(Event *bombEventPtr[], int left, int mid, int right) {

    int lsize = mid - left + 1;
    int rsize = right - mid;

    // Create temp arrays
    Event *L[lsize], *R[rsize];

    // Copy data to temp arrays
    for(int i = 0; i < lsize; ++i)
        L[i] = bombEventPtr[left + i];
    for(int i = 0; i < rsize; ++i)
        R[i] = bombEventPtr[mid + 1 + i];

    int lid = 0, rid = 0, id = left;

    while(lid < lsize && rid < rsize) {

        if(L[lid] -> value <= R[rid] -> value) {
            if(L[lid] -> type == 'P')
                L[lid] -> damage -= Memory::get(L[lid] -> location);

            bombEventPtr[id++] = L[lid++];
        }
        else {
            if(R[rid] -> type == 'A')
                Memory::add(R[rid] -> left, R[rid] -> right, R[rid] -> damage);

            bombEventPtr[id++] = R[rid++];
        }
    }

    // Copy the remaining elements of R[]
    while(rid < rsize) {
        if(R[rid] -> type == 'A')
            Memory::add(R[rid] -> left, R[rid] -> right, R[rid] -> damage);
        bombEventPtr[id++] = R[rid++];
    }

    // Copy the remaining elements of L[]
    while(lid < lsize) {
        if(L[lid] -> type == 'P')
            L[lid] -> damage -= Memory::get(L[lid] -> location);
        bombEventPtr[id++] = L[lid++];
    }

    if(id != right + 1)
        cout << id << " " << left << " " << right << endl;
    assert(id == right + 1);

    // Add back

    for(int lid = 0; lid < lsize; ++lid) {
        if(L[lid] -> type == 'P') {
            L[lid] -> damage += Memory::get(L[lid] -> location);
        }
    }
}


// Run events within the time period between left and right

void runEvents(Event *bombEventPtr[], int left, int right) {

    // Use brute force method if less than 3
    if(right - left <= 2) {
        bruteForce(bombEventPtr, left, right);
        return;
    }

    int mid = (left + right) / 2;

    // Run the subproblems
    runEvents(bombEventPtr, mid + 1, right);
    runEvents(bombEventPtr, left, mid);

    // Merge two subproblems
    merge(bombEventPtr, left, mid, right);

    return;
}

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int size, numOfEvents;

    // Create an array of ptrs that point to every event

    Event bombEvents[MAXE];
    Event *bombEventPtr[MAXE];

    // Read in size and number of events

    cin >> size >> numOfEvents;

    // Save data in the bombEvents timeline

    for(int i = 0; i < numOfEvents; ++i) {
        cin >> bombEvents[i].type;

        // Player event
        if(bombEvents[i].type == 'P') {
            cin >> bombEvents[i].location >> bombEvents[i].value;
            bombEvents[i].damage = 0;
        }

        // Attack event
        else if(bombEvents[i].type == 'A') {
            cin >> bombEvents[i].left >> bombEvents[i].right >>
                   bombEvents[i].value >> bombEvents[i].damage;
        }

        // Unknown event
        else
            cout << "Unknown event" << endl;

        // Point ptr to designated event
        bombEventPtr[i] = &bombEvents[i];

    }

    runEvents(bombEventPtr, 0, numOfEvents - 1);

    //Output the damaged inflicted on different players

    for(int i = 0; i < numOfEvents; ++i)
        if(bombEvents[i].type == 'P')
            cout << bombEvents[i].damage << endl;

    return 0;
}
