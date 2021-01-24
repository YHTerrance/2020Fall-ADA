#include <iostream>
#include <cassert>
#include <cmath>
#include <deque>

using namespace std;

#define MAXN 100001
#define MAXS 1000000001

typedef pair<int, long long int> pii;

int main() {
    long long int N, budget, bonus_A, bonus_B;

    int suitability[MAXN + 1] = {};

    // Store max of every result column
    long long int resultMax = 0;

    // Read input
    cin >> N >> budget >> bonus_A >> bonus_B;

    // Start from 1 to N
    for(int i = 0; i < N; ++i)
        cin >> suitability[i];

    // Initialize the DP table as (0, 0) = (current value, last jumped tower_id)
    long long int towers[MAXN][2];

    for(int i = 0; i < N; ++i) {
        towers[i][0] = suitability[i];

        if(towers[i][0] > resultMax)
            resultMax = towers[i][0];
    }


    deque<pii> bonus_queue;
    long long int maxInBonus, maxOutOfBonus, value;
    int col1 = 0, col2 = 1;


    for(int k = 1; k < budget; ++k) {

        // Max of the region that has no bonus
        maxOutOfBonus = 0;

        bonus_queue.clear();

        for(int i = k; i < N; ++i) {

            value = towers[i - 1][col1] + bonus_A * (i - 1);

            // Remove smaller ones from front
            while(!bonus_queue.empty() && bonus_queue.front().second <= value)
                bonus_queue.pop_front();

            // Add new value into queue
            bonus_queue.push_front(make_pair(i - 1, value));

            // Remove one from the back if it is over filled
            while(i - bonus_queue.back().first > bonus_B)
                bonus_queue.pop_back();

            // Max value in bonus scope
            maxInBonus = bonus_A * bonus_B - bonus_A * i + bonus_queue.back().second;

            // Max value out of bonus area
            if(i - bonus_B - 1 >= k - 1 && towers[i - bonus_B - 1][col1] > maxOutOfBonus)
                maxOutOfBonus = towers[i - bonus_B - 1][col1];

            towers[i][col2] = suitability[i] + std::max(maxInBonus, maxOutOfBonus);

            if(towers[i][col2] > resultMax)
                resultMax = towers[i][col2];
        }
        swap(col1, col2);
    }

    cout << resultMax << endl;

    return 0;
}
